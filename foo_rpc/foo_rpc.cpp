#include <string>
#include <deque>
#include <future>

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


DECLARE_COMPONENT_VERSION(
"foo_rpc",
"0.0.1",
"Access foobar2000's internal API programmatically..\n"
);

class foobar2000api : public initquit {
private:
  DWORD ThreadID;
  
  // TODO: get this from a config file
  PipeListener listener = PipeListener("\\\\.\\pipe\\foobar2000");

public:

  DWORD listen_commands() {

    vector<char> received;    
    auto dispatcher = foobar::MethodDispatcher();

    while (true) {
      
      PipeConnection connection = listener.accept();      
      Result<tuple<DWORD, vector<char>>> result = connection.recv();

      if (result.isFailed()) {
        logToFoobarConsole("Failed receiving data from pipe %d", result.error());
        return result.error();
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
      connection.close();
    }
    return 0;

  }

  static DWORD WINAPI named_pipe_thread(void* Param)
  {
    foobar2000api* This = (foobar2000api*)Param;
    return This->listen_commands();
  }

  virtual void on_init()
  {
    // Start the named pipe server
    CreateThread(NULL, 0, named_pipe_thread, (void*) this, 0, &ThreadID);
  }

  virtual void on_quit()
  {
    /*
    TODO DisconnectNamedPipe(pipe)?;
    */
    listener.close();
  }
};


initquit_factory_t<foobar2000api> g_foo;

// The module attribute is specified in order to implement DllMain,
// DllRegisterServer and DllUnregisterServer
[module(dll, name = "foo_rpc", helpstring = "Foobar automation component")];
[emitidl];