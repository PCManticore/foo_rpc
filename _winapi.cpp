#pragma once
#include <assert.h>
#include <string>
#include <tuple>

#include "_winapi.h"
#include "logging.h"
#include "result.h"

using namespace std;

OverlappedObject *new_overlapped(HANDLE handle)
{
  OverlappedObject *self = new OverlappedObject();
  self->handle = handle;
  self->pending = 0;
  self->completed = 0;
  memset(&self->overlapped, 0, sizeof(OVERLAPPED));
  /* Manual reset, initially non-signalled */
  self->overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
  return self;
}

DWORD create_pipe(std::string pipeAddress,
  HANDLE* pipeOut,
  bool isFirst) {

  DWORD flags = PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED;
  if (isFirst) {
    flags |= FILE_FLAG_FIRST_PIPE_INSTANCE;
  }

  *pipeOut = CreateNamedPipe(
    std::wstring(pipeAddress.begin(), pipeAddress.end()).c_str(),
    flags,
    PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
    PIPE_UNLIMITED_INSTANCES,
    1024 * 16,
    1024 * 16,
    NMPWAIT_WAIT_FOREVER,
    NULL);

  if (pipeOut == INVALID_HANDLE_VALUE) {
    logToFoobarConsole(
      "Failed creating named pipe. "
      "The operation failed with error %d.",
      GetLastError());
    return OPERATION_FAILED;
  }
  return OPERATION_SUCCESS;
}

Result<OverlappedObject*> connect_pipe(HANDLE handle) {
  DWORD success;

  OverlappedObject *overlapped = new_overlapped(handle);
  success = ConnectNamedPipe(
    handle,
    overlapped ? &overlapped->overlapped : NULL);

  if (!overlapped) {
    logToFoobarConsole(
      "Cannot create an overlapped object. "
      "The operation failed with error %d.",
      GetLastError());
    return Result<OverlappedObject*>::withError(GetLastError());
  }

  int err = GetLastError();
  /* Overlapped ConnectNamedPipe never returns a success code */
  assert(success == 0);
  if (err == ERROR_IO_PENDING) {
    // TODO HOW TO HANDLE THIS?
    overlapped->pending = 1;
  }
  else if (err == ERROR_PIPE_CONNECTED) {
    SetEvent(overlapped->overlapped.hEvent);
  }
  else {
    logToFoobarConsole("Could not connect to the named pipe. "
      "The operation failed with error %d.", err);
    return Result<OverlappedObject*>::withError(err);
  }
  return Result<OverlappedObject*>(overlapped);
}

DWORD wait_overlapped_event(OverlappedObject* overlapped) {
  HANDLE events[1] = { overlapped->event };
  return WaitForMultipleObjects(1, events, FALSE, INFINITE);
}

Result<DWORD> get_overlapped_event(OverlappedObject * overlapped) {
  DWORD result;
  DWORD error;
  DWORD transferred = 0;

  result = GetOverlappedResult(
    overlapped->handle,
    &overlapped->overlapped,
    &transferred,
    true);

  error = result ? ERROR_SUCCESS : GetLastError();

  switch (error) {
  case ERROR_SUCCESS:
  case ERROR_MORE_DATA:
  case ERROR_OPERATION_ABORTED:
    overlapped->completed = 1;
    overlapped->pending = 0;
    break;
  case ERROR_IO_INCOMPLETE:
    break;
  default:
    overlapped->pending = 0;
    return Result<DWORD>::withError(error);
  }
  return Result<DWORD>(error);
}

Result<tuple<DWORD, DWORD>> peek_named_pipe(HANDLE handle, char * buf, int size) {
  DWORD nread, navail, nleft;
  BOOL ret;

  if (size) {
    ret = PeekNamedPipe(handle, buf, size, &nread, &navail, &nleft);
  }
  else {
    ret = PeekNamedPipe(handle, NULL, 0, NULL, &navail, &nleft);
  }

  if (!ret) {
    return Result<tuple<DWORD, DWORD>>::withError(GetLastError());
  }
  return Result<tuple<DWORD, DWORD>>(make_tuple(navail, nleft));

}

Result<DWORD> get_more_data(HANDLE handle, char * buffer, int maxsize) {
  DWORD navail, nleft;
  Result<tuple<DWORD, DWORD>> result = peek_named_pipe(handle, buffer, maxsize);
  if (result.isFailed()) {
    return Result<DWORD>::withError(result.error());
  }
  tie(navail, nleft) = result.result();

  assert(nleft > 0);
  // TODO recv bytes? will be circular dep since it will be called from recv_bytes?
  return recv_bytes(handle, buffer, navail);
}

Result<tuple<DWORD, DWORD>> write_to_pipe(HANDLE handle,
  OverlappedObject * overlapped,
  const char * writeBuffer,
  int len) {
  DWORD written;
  BOOL ret;
  DWORD err;

  ret = WriteFile(handle, writeBuffer, len, &written,
    overlapped ? &overlapped->overlapped : NULL);
  err = ret ? 0 : GetLastError();

  if (!ret) {
    if (err == ERROR_IO_PENDING)
      overlapped->pending = 1;
    else {
      return Result<tuple<DWORD, DWORD>>::withError(err);
    }
  }
  return Result<tuple<DWORD, DWORD>>(make_tuple(err, written));
}


Result<tuple<DWORD, DWORD>> read_from_pipe(HANDLE handle, int size,
  OverlappedObject * overlapped,
  char * readBuffer) {

  DWORD nread;
  DWORD err;
  BOOL ret;

  ret = ReadFile(
    handle, readBuffer, size, &nread,
    overlapped ? &overlapped->overlapped : NULL);

  err = ret ? 0 : GetLastError();
  if (!ret) {
    if (err == ERROR_IO_PENDING)
      // TODO: what to do with this pending field?
      overlapped->pending = 1;
    else if (err != ERROR_MORE_DATA) {
      logToFoobarConsole("Cannot read from pipe. "
        "The operation failed with error %d.", err);
      return Result<tuple<DWORD, DWORD>>::withError(err);
    }
  }
  return Result<tuple<DWORD, DWORD>>(make_tuple(err, nread));
}

Result<DWORD> send_bytes(HANDLE handle, const char * writeBuffer, int len) {
  DWORD error;
  DWORD written;
  DWORD waitres;
  DWORD lastError;

  OverlappedObject * overlapped = new_overlapped(handle);
  if (!overlapped) {
    logToFoobarConsole("Cannot get an overlapped object: %d.",
      GetLastError());
    return Result<DWORD>::withError(GetLastError());
  }

  Result<tuple<DWORD, DWORD>> result = write_to_pipe(handle, overlapped, writeBuffer, len);
  if (result.isFailed()) {
    logToFoobarConsole("Failed writing to pipe %d", result.error());
    return Result<DWORD>::withError(result.error());
  }

  tie(error, written) = result.result();
  if (error == ERROR_IO_PENDING) {
    waitres = wait_overlapped_event(overlapped);
    assert(waitres == WAIT_OBJECT_0);
  }

  Result<DWORD> resultOverlapped = get_overlapped_event(overlapped);
  if (resultOverlapped.isFailed()) {
    logToFoobarConsole("Getting overlapped event failed with %d.",
      resultOverlapped.error());
    return resultOverlapped;
  }
  lastError = resultOverlapped.result();
  assert(lastError == 0);
  // TODO: assert nwritten == len(buf)	
  return Result<DWORD>(lastError);

}

Result<DWORD> recv_bytes(HANDLE handle, char * readBuffer, int size) {
  DWORD waitres;
  DWORD error;
  DWORD lastError;
  DWORD nread;

  OverlappedObject * overlapped = new_overlapped(handle);
  if (!overlapped) {
    logToFoobarConsole("Cannot get an overlapped object: %d.",
      GetLastError());
    return Result<DWORD>::withError(GetLastError());
  }
  Result<tuple<DWORD, DWORD>> result = read_from_pipe(handle, size,
    overlapped,
    readBuffer);
  if (result.isFailed()) {
    logToFoobarConsole("Failed receiving from pipe %d", result.error());
    return Result<DWORD>::withError(result.error());
  }

  tie(error, nread) = result.result();
  if (error == ERROR_IO_PENDING) {
    waitres = wait_overlapped_event(overlapped);
    assert(waitres == WAIT_OBJECT_0);
  }

  Result<DWORD> resultOverlapped = get_overlapped_event(overlapped);
  if (resultOverlapped.isFailed()) {
    logToFoobarConsole("Getting overlapped event failed with %d.",
      resultOverlapped.error());
    return resultOverlapped;
  }
  lastError = resultOverlapped.result();

  switch (lastError) {
  case ERROR_MORE_DATA:
    // TODO: implement this
    break;
  case ERROR_SUCCESS:
    // Operation succeeded.
    return Result<DWORD>(nread);
  default:
    return Result<DWORD>::withError(lastError);
  }
  return Result<DWORD>::withError(lastError);
}


/*

TODO:

1. pasez bufferii dintr-o parte in alta sau ii pun in Overlapped
2. folosesc char * sau altceva
3. acolo unde intorc DWORD (recv_bytes) ar trebui sa intorc Result<DWORD>,
   chiar daca intorc deja vreun result.error() prin cod (ce e resultul lui recv_bytes, eroare
   sau numar de bytes cititi?)
4. cum implementez more data
5. use waitforsingleobject or waitformultipleobjects?
6. move pipelistener into its own file
7. make an event object similar to asyncobj
8. think about a protocolbetween foo_automation and foobar_callback, including proxying
    of methods from former to the latter
9. receive / write json at th epipe layer
10. API for passing callbacks between threads
11. configuration object for various settings
*/