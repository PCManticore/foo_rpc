#include <condition_variable>
#include <string>
#include <deque>
#include <thread>

#include "local_exceptions.h"
#include "event.h"
#include "logging.h"
#include "percolate.h"
#include "pipe.h"
#include "_winapi.h"
#include "stdafx.h"

#include "rpcapi/dispatch.h"
#include "rpcapi/serialization/base.h"

using namespace std;

#define RPC_ADDRESS "\\\\.\\pipe\\foobar2000"

DECLARE_COMPONENT_VERSION(
"foo_rpc",
"0.0.1",
"Access foobar2000's internal API programmatically..\n"
);

class Thread {
private:
  std::thread thr;
  HANDLE event;

public:
  Thread(std::thread & thread, HANDLE & eventObj) {
    thr.swap(thread);
    event = eventObj;
  }

  void join() {
    SetEvent(event);
    thr.join();
  }
};

class foobar2000api : public initquit {
private:
  vector<Thread> underlying_threads;

  // TODO: need createnewthread API?
  DWORD ThreadID;
  PipeListener listener = PipeListener(RPC_ADDRESS);

  bool is_signaled(HANDLE event) {
    auto res = WaitForSingleObject(event, 0);
    switch (res) {
    case WAIT_TIMEOUT:
      return false;
    case WAIT_OBJECT_0:
      return true;
    default:
      return true;
    }
  }

  void process_incoming_connection(PipeConnection connection, HANDLE event) {
    vector<char> received;
    auto dispatcher = foobar::MethodDispatcher();

    while (!is_signaled(event)) {
      Result<tuple<DWORD, vector<char>>> result = connection.recv();

      if (result.isFailed()) {
        logToFoobarConsole("Failed receiving data from pipe %d", result.error());
        connection.close();
        break;
      }

      tie(ignore, received) = result.result();

      try {
        serialization::Payload rpc_result = dispatcher.dispatch(received);
        connection.send(rpc_result.data, rpc_result.size);
      }
      catch (RPCException & e) {
        logToFoobarConsole("Error while dispatching: %s", e.what());
        connection.send(e.what());
      }
    }

  }

  static DWORD WINAPI named_pipe_thread(void* Param) {
    foobar2000api* This = (foobar2000api*)Param;
    This->listen_commands();
    return 0;
  }

public:

  void listen_commands() {

    while (true) {

      try {
        PipeConnection connection = listener.accept();
        HANDLE event = CreateEvent(NULL, TRUE, FALSE, NULL);
        underlying_threads.push_back(Thread(
          std::thread([&] {
          process_incoming_connection(connection, event);
        }), event));
      }
      catch (PipeException & e) {
        logToFoobarConsole("Failed connecting to pipe with error %s", e.what());
        return;
      }

    }
  }



  void on_init()
  {
    // Start the named pipe server
    CreateThread(NULL, 0, named_pipe_thread, (void*) this, 0, &ThreadID);
  }

  void on_quit()
  {
    listener.close();
    for (auto &thread : underlying_threads) {
      thread.join();
    }
  }
};

initquit_factory_t<foobar2000api> g_foo;

// The module attribute is specified in order to implement DllMain,
// DllRegisterServer and DllUnregisterServer
[module(dll, name = "foo_rpc", helpstring = "Foobar automation component")];
[emitidl];