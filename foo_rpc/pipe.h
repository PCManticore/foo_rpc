#pragma once
#include <deque>
#include <string>

#include "logging.h"
#include "_winapi.h"

#define INITIAL_READ_SIZE 128

using namespace std;


class PipeConnection {
private:
  OverlappedObject overlapped;

public:


  PipeConnection(OverlappedObject & overlappedObj) : overlapped(overlappedObj) {}

  Result<DWORD> send(string bytes, int length) {
    return send_bytes(overlapped.handle, bytes, length);
  }

  Result<DWORD> send(string bytes) {
    return send(bytes, bytes.length());
  }

  Result<tuple<DWORD, vector<char>>> recv() {
    DWORD lastError, nread = 0;
    vector<char> moreData;
    vector<char> buffer(INITIAL_READ_SIZE);

    Result<tuple<DWORD, DWORD>> result = recv_bytes(overlapped.handle, &buffer[0], INITIAL_READ_SIZE);
    if (result.isFailed()) {
      logToFoobarConsole("Failed receiving bytes %d", result.error());
      return Result<tuple<DWORD, vector<char>>>::withError(result.error());
    }

    tie(lastError, nread) = result.result();
    if (lastError == ERROR_MORE_DATA) {
      Result<tuple<DWORD, vector<char>>> moreDataResult = get_more_data(overlapped.handle);
      if (moreDataResult.isFailed()) {
        logToFoobarConsole("Failed retrieving more data %d", moreDataResult.error());
        return Result<tuple<DWORD, vector<char>>>::withError(moreDataResult.error());
      }
      
      tie(ignore, moreData) = moreDataResult.result();
      
      buffer.insert(end(buffer), begin(moreData), end(moreData));

    }
    return Result<tuple<DWORD, vector<char>>>(make_tuple(nread + buffer.size(), buffer));
  }

  void close() {
    DWORD bytes;

    int err = GetLastError();

    if (overlapped.pending) {
      if (CancelIoEx(overlapped.handle, &overlapped.overlapped) &&
        GetOverlappedResult(overlapped.handle, &overlapped.overlapped, &bytes, TRUE))
      {
        /* The operation is no longer pending -- nothing to do. */
      }
      else
      {
        /* The operation is still pending, but the process is
        probably about to exit, so we need not worry too much
        about memory leaks.
        */
        CloseHandle(overlapped.overlapped.hEvent);
        SetLastError(err);
        return;
      }
    }

    SetLastError(err);
    //CloseHandle(overlapped.handle);
    //CloseHandle(overlapped.overlapped.hEvent);
  }

};

class PipeListener {
public:
  PipeListener(string address) : pipeAddress(address) {
    create_named_pipe(true);
  }

  PipeConnection accept() {

    HANDLE handle;

    create_named_pipe(false);
    handle = handles.front();
    handles.pop_front();

    Result<OverlappedObject> result = connect_pipe(handle);
    if (result.isFailed()) {
      std::string msg = tfm::format(
        "Could not connect to pipe. Failed with error %d",
        result.error());      
      throw PipeException(msg);
    }

    OverlappedObject & overlapped = result.result();
    wait_overlapped_event(overlapped);

    Result<DWORD> overlappedResult = get_overlapped_result(overlapped);
    if (overlappedResult.isFailed() || overlappedResult.result() != 0) {
      std::string msg = tfm::format(
        "Getting overlapped event failed with %d.",
         overlappedResult.error());
      throw PipeException(msg);      
    }

    return PipeConnection(overlapped);
  }

  void close() {
    while (handles.size() > 0) {
      HANDLE handle = handles.front();
      handles.pop_front();
      DisconnectNamedPipe(handle);
    }
  }

private:
  deque<HANDLE> handles = {};
  string pipeAddress;

  /*Creates a named pipe

  Returns 0 when the pipe couldn't be created, 1 otherwise.
  */
  DWORD create_named_pipe(bool isFirst) {
    HANDLE pipe;
    DWORD result = create_pipe(pipeAddress, &pipe, isFirst);
    if (result == OPERATION_SUCCESS) {
      handles.push_back(pipe);
    }
    return result;
  }
};