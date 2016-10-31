#include <algorithm>
#include <iterator>
#include <deque>
#include <string>
#include <thread>

#include "local_exceptions.h"
#include "event.h"
#include "logging.h"
#include "percolate.h"
#include "pipe.h"
#include "_winapi.h"
#include "stdafx.h"

#include "safequeue.h"
#include "rpcapi/dispatch.h"
#include "rpcapi/serialization/base.h"

using namespace std;

#define RPC_ADDRESS "\\\\.\\pipe\\foobar2000"


class foobar2000api : public initquit {
private:
  Event stopCollectorEvent;
  std::thread collectorThread;
  std::vector<std::tuple<std::thread, Event>> underlying_threads;
  std::vector<PipeConnection> trackedConnections;
  thread_util::Queue<std::tuple<PipeConnection, bool>> connectionsQueue;


  // TODO: need createnewthread API?
  DWORD ThreadID;
  PipeListener listener = PipeListener(RPC_ADDRESS);

  static void process_incoming_connection(PipeConnection connection, Event event) {
    vector<char> received;
    auto dispatcher = foobar::MethodDispatcher();

    while (!event.isReady()) {
      Result<tuple<DWORD, vector<char>>> result = connection.recv();

      if (result.isFailed()) {
        logToFoobarConsole("Failed receiving data from pipe %d", result.error());
        int is_closed = connection.is_closed();
        connection.close();
        break;
      }

      tie(ignore, received) = result.result();

      try {
        serialization::Payload rpc_result = dispatcher.dispatch(received);
        connection.send(rpc_result.data, rpc_result.size);
      }
      catch (RPCException & e) {
        auto response = serialization::serializer.packed(
          make_tuple(1, e.what())
        );
        logToFoobarConsole("Error while dispatching: %s", e.what());        
        connection.send(response.data, response.size);
      }
    }

  }

  static DWORD WINAPI named_pipe_thread(void* Param) {
    foobar2000api* This = (foobar2000api*)Param;
    This->listen_commands();
    return 0;
  }

  static void close_tracked_connections(Event stopEvent,
                                        thread_util::Queue<std::tuple<PipeConnection, bool>> * queue) {
    bool is_last;
    PipeConnection conn;

    while (!stopEvent.isReady()) {
      std::tie(conn, is_last) = queue->pop();
      conn.close();
    }
  }

public:

  void listen_commands() {

    while (true) {

      try {
        PipeConnection connection = listener.accept();
        Event event;

        underlying_threads.push_back(std::make_tuple(
          std::thread([connection, event] { process_incoming_connection(connection, event); }),
          event));
        trackedConnections.push_back(connection);
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
    collectorThread = std::thread(
      close_tracked_connections,
      stopCollectorEvent,
      &connectionsQueue);
  }

  void on_quit()
  {
    listener.close();

    // Track the non-closed connections for closing them up.
    for (auto iter = trackedConnections.begin(); iter != trackedConnections.end(); ++iter) {
      bool is_last = std::next(iter) == trackedConnections.end();
      if (!iter->is_closed()) {
        connectionsQueue.push(std::make_tuple((*iter), is_last));
      }
    }

    /// Notify the collector thread to close after finishing processing
    // the non-closed connections.
    stopCollectorEvent.set();
    collectorThread.join();

    for (auto &thread : underlying_threads) {
      std::get<1>(thread).set();
      std::get<0>(thread).join();
    }

  }
};

initquit_factory_t<foobar2000api> g_foo;

VALIDATE_COMPONENT_FILENAME("foo_rpc.dll");

DECLARE_COMPONENT_VERSION(
"foo_rpc",
"0.1.0",
"Access foobar2000's internal API programmatically..\n"
);