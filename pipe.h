#pragma once
#include <deque>
#include <string>

#include "logging.h"
#include "_winapi.h"

using namespace std;

// TODO: move in a .cpp file?
class PipeConnection {
public:
  // TODO: should be public?
  HANDLE handle;

  PipeConnection(HANDLE pipeHandle) : handle(pipeHandle) {}

  Result<DWORD> send(const char * bytes) {
    return send_bytes(handle, bytes, strlen(bytes));
  }

  DWORD recv() {
    // TODO
    return 0;
  }

  void close() {
    CloseHandle(handle);
  }

};

class PipeListener {
public:
  PipeListener(string address) : pipeAddress(address) {
    createNamedPipe(true);
  }

  PipeConnection accept() {

    HANDLE handle;
    DWORD lastError;
    BOOL success;

    createNamedPipe(false);
    handle = handles.front();
    handles.pop_front();

    Result<OverlappedObject*> result = connect_pipe(handle);
    if (result.isFailed()) {
      logToFoobarConsole("Could not connect to pipe %d", result.error());
      return NULL;
    }
    OverlappedObject* overlapped = result.result();

    // TODO handle exceptional case
    int res = wait_overlapped_event(overlapped);

    Result<DWORD> overlappedResult = get_overlapped_event(overlapped);
    if (overlappedResult.isFailed()) {
      logToFoobarConsole("Getting overlapped event failed with %d.",
        overlappedResult.error());
      return NULL;
    }

    return PipeConnection(handle);
  }

  void close() {
    for (auto handle : handles) {
      CloseHandle(handle);
    }
  }

private:
  deque<HANDLE> handles = {};
  string pipeAddress;

  /*Creates a named pipe

  Returns 0 when the pipe couldn't be created, 1 otherwise.
  */
  DWORD createNamedPipe(bool isFirst) {
    HANDLE pipe;
    DWORD result = create_pipe(pipeAddress, &pipe, isFirst);
    if (result == OPERATION_SUCCESS) {
      handles.push_back(pipe);
    }
    return result;
  }
};