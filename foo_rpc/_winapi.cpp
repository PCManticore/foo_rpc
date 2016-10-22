#pragma once
#include <assert.h>
#include <string>
#include <tuple>
#include <vector>

#include "_winapi.h"
#include "logging.h"
#include "result.h"

using namespace std;

OverlappedObject new_overlapped(HANDLE handle)
{
  OverlappedObject self;
  self.handle = handle;
  self.pending = 0;
  self.completed = 0;
  memset(&self.overlapped, 0, sizeof(OVERLAPPED));
  /* Manual reset, initially non-signalled */
  self.overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
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

Result<OverlappedObject> connect_pipe(HANDLE handle) {
  DWORD success;

  OverlappedObject overlapped = new_overlapped(handle);
  success = ConnectNamedPipe(handle, &overlapped.overlapped);

  int err = GetLastError();
  /* Overlapped ConnectNamedPipe never returns a success code */
  assert(success == 0);
  if (err == ERROR_IO_PENDING) {
    overlapped.pending = 1;
  }
  else if (err == ERROR_PIPE_CONNECTED) {
    SetEvent(overlapped.overlapped.hEvent);
  }
  else {
    logToFoobarConsole(
      "Could not connect to the named pipe. "
      "The operation failed with error %d.", err);
    return Result<OverlappedObject>::withError(err);
  }
  return Result<OverlappedObject>(overlapped);
}

DWORD wait_overlapped_event(OverlappedObject & overlapped) {
  return WaitForSingleObject(overlapped.overlapped.hEvent, INFINITE);
}

Result<DWORD> get_overlapped_result(OverlappedObject & overlapped) {
  DWORD result;
  DWORD error;
  DWORD transferred = 0;

  result = GetOverlappedResult(
    overlapped.handle,
    &overlapped.overlapped,
    &transferred,
    true);

  error = result ? ERROR_SUCCESS : GetLastError();

  switch (error) {
  case ERROR_SUCCESS:
  case ERROR_MORE_DATA:
  case ERROR_OPERATION_ABORTED:
    overlapped.completed = 1;
    overlapped.pending = 0;
    break;
  case ERROR_IO_INCOMPLETE:
    break;
  default:
    overlapped.pending = 0;
    return Result<DWORD>::withError(error);
  }
  return Result<DWORD>(error);
}

Result<tuple<DWORD, DWORD>> write_to_pipe(HANDLE handle,
                                          OverlappedObject & overlapped,
                                          string writeBuffer,
                                          int len) {
  DWORD written;
  BOOL ret;
  DWORD err;

  ret = WriteFile(handle, writeBuffer.c_str(), len, &written,
                  &overlapped.overlapped);
  err = ret ? 0 : GetLastError();

  if (!ret) {
    if (err == ERROR_IO_PENDING)
      overlapped.pending = 1;
    else {
      return Result<tuple<DWORD, DWORD>>::withError(err);
    }
  }
  return Result<tuple<DWORD, DWORD>>(make_tuple(err, written));
}


Result<tuple<DWORD, DWORD>> read_from_pipe(HANDLE handle,
                                           OverlappedObject & overlapped,
                                           char * readBuffer,
                                           int size) {

  DWORD nread;
  DWORD err;
  BOOL ret;

  ret = ReadFile(
    handle, readBuffer, size, &nread,
    &overlapped.overlapped);

  err = ret ? 0 : GetLastError();
  if (!ret) {
    if (err == ERROR_IO_PENDING)
      overlapped.pending = 1;
    else if (err != ERROR_MORE_DATA) {
      logToFoobarConsole("Cannot read from pipe. "
                         "The operation failed with error %d.", err);
      return Result<tuple<DWORD, DWORD>>::withError(err);
    }
  }
  return Result<tuple<DWORD, DWORD>>(make_tuple(err, nread));
}

Result<tuple<DWORD, DWORD>> peek_named_pipe(HANDLE handle) {
  DWORD navail, nleft;
  BOOL ret;

  ret = PeekNamedPipe(handle, NULL, 0, NULL, &navail, &nleft);

  if (!ret) {
    return Result<tuple<DWORD, DWORD>>::withError(GetLastError());
  }
  return Result<tuple<DWORD, DWORD>>(make_tuple(navail, nleft));

}

Result<tuple<DWORD, vector<char>>> get_more_data(HANDLE handle) {
  DWORD nleft;
  DWORD nread;
  Result<tuple<DWORD, DWORD>> resultPeek = peek_named_pipe(handle);
  if (resultPeek.isFailed()) {
    return Result<tuple<DWORD, vector<char>>>::withError(resultPeek.error());
  }
  tie(ignore, nleft) = resultPeek.result();

  assert(nleft > 0);

  OverlappedObject & overlapped = new_overlapped(handle);

  std::vector<char> buffer(nleft);
  Result<tuple<DWORD, DWORD>> result = read_from_pipe(handle, overlapped, &buffer[0], nleft);
  if (result.isFailed()) {
    return Result<tuple<DWORD, vector<char>>>::withError(result.error());
  }

  tie(ignore, nread) = result.result();
  return Result<tuple<DWORD, vector<char>>>(make_tuple(nread, buffer));
}

Result<DWORD> send_bytes(HANDLE handle, string writeBuffer, int len) {
  DWORD error;
  DWORD written;
  DWORD waitres;
  DWORD lastError;

  OverlappedObject overlapped = new_overlapped(handle);

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

  Result<DWORD> resultOverlapped = get_overlapped_result(overlapped);
  if (resultOverlapped.isFailed()) {
    logToFoobarConsole("Getting overlapped event failed with %d.",
      resultOverlapped.error());
    return resultOverlapped;
  }
  lastError = resultOverlapped.result();
  assert(lastError == 0);
  assert(written == len);
  return Result<DWORD>(lastError);

}

Result<tuple<DWORD, DWORD>> recv_bytes(HANDLE handle, char * readBuffer, int size) {
  DWORD waitres;
  DWORD error;
  DWORD lastError;
  DWORD nread;

  OverlappedObject overlapped = new_overlapped(handle);

  Result<tuple<DWORD, DWORD>> result = read_from_pipe(handle, overlapped, readBuffer, size);
  if (result.isFailed()) {
    logToFoobarConsole("Failed receiving from pipe %d", result.error());
    return result;
  }

  tie(error, nread) = result.result();
  if (error == ERROR_IO_PENDING) {
    waitres = wait_overlapped_event(overlapped);
    assert(waitres == WAIT_OBJECT_0);
  }

  Result<DWORD> resultOverlapped = get_overlapped_result(overlapped);
  if (resultOverlapped.isFailed()) {
    logToFoobarConsole("Getting overlapped event failed with %d.",
      resultOverlapped.error());
    return Result<tuple<DWORD, DWORD>>::withError(resultOverlapped.error());;
  }
  lastError = resultOverlapped.result();

  switch (lastError) {
  case ERROR_MORE_DATA:    
  case ERROR_SUCCESS:
    // Operation succeeded or needs additional operations.
    return Result<tuple<DWORD, DWORD>>(make_tuple(lastError, nread));
  default:
    return Result<tuple<DWORD, DWORD>>::withError(lastError);
  }
  return Result<tuple<DWORD, DWORD>>::withError(lastError);
}
