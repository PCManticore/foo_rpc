#include <string>
#include <deque>
#include <future>

#include "asyncobj.h"
#include "event.h"
#include "logging.h"
#include "pipe.h"
#include "_winapi.h"
#include "stdafx.h"

using namespace std;


DECLARE_COMPONENT_VERSION(
"foo_automation",
"0.0.1",
"Access foobar2000's internal API programmatically..\n"
);

class foobar2000api : public initquit {
private:
  DWORD ThreadID;
public:

  DWORD create_named_pipe() {

    DWORD res;
    // TODO: get this from a config file
    PipeListener listener("\\\\.\\pipe\\foobar2000");
    while (true) {
      logToFoobarConsole("acception new client %d", 2222);
      PipeConnection connection = listener.accept();
      logToFoobarConsole("client accepted");

      char buffer[1000] = "";
      logToFoobarConsole("try to recv bytes from him");
      Result<DWORD> result = recv_bytes(connection.handle, buffer, 256);
      if (result.isFailed()) {
        logToFoobarConsole("Failed receiving data from pipe %d", result.error());
        return result.error();
      }

      logToFoobarConsole("finished recving bytes from him");
      logToFoobarConsole(string(buffer));

      // TO REMOVE
      /*static_api_ptr_t<playlist_manager> pm;
      static_api_ptr_t<playback_control> pc;
      metadb_handle_ptr ptr;
      DWORD length = 0;
      if (pc->get_now_playing(ptr))
      {
        length = ptr->get_length();
      }
      string songLength = to_string(length);

      future fut;
      static_api_ptr_t<main_thread_callback_manager>()->add_callback(
        new service_impl_t<console_debug_callback>(fut)
      );

      */
      Event event;
      static_api_ptr_t<main_thread_callback_manager>()->add_callback(
        new service_impl_t<current_playlist_callback>(event)
      );
      event.wait();

      logToFoobarConsole("is ready %s", event.isReady());
      send_bytes(connection.handle, "claudiu", 7);

    }

    /*
    TODO
    DisconnectNamedPipe(pipe);
    */
    return 0;

  }

  static DWORD WINAPI named_pipe_thread(void* Param)
  {
    foobar2000api* This = (foobar2000api*)Param;
    return This->create_named_pipe();
  }

  virtual void on_init()
  {
    // Start the named pipe server
    CreateThread(NULL, 0, named_pipe_thread, (void*) this, 0, &ThreadID);
  }

  virtual void on_quit()
  {

  }
};


initquit_factory_t<foobar2000api> g_foo;

// The module attribute is specified in order to implement DllMain,
// DllRegisterServer and DllUnregisterServer
[module(dll, name = "foo_automation", helpstring = "Foobar automation component")];
[emitidl];