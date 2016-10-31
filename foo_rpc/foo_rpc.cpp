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
  Event stopNamedPipeThreadEvent;
  std::thread namedPipeThread;
  std::thread collectorThread;
  PipeConnection _dummyConnection;
  std::vector<std::tuple<std::thread, Event>> underlying_threads;
  std::vector<PipeConnection> trackedConnections;
  thread_util::Queue<PipeConnection> connectionsQueue;


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

  static void listen(void * param) {
    foobar2000api* This = (foobar2000api*)param;
    This->listen_commands();
  }

  static void close_tracked_connections(Event stopEvent,
                                        thread_util::Queue<PipeConnection> * queue,
                                        void * param) {
    PipeConnection conn;
    foobar2000api * thisobj = (foobar2000api*)param;

    while (!stopEvent.isReady()) {
      conn = queue->pop();
      if (conn == thisobj->_dummyConnection) {
        conn.close();
        return;
      }

      conn.close();
    }
  }

public:

  void listen_commands() {

    while (!stopNamedPipeThreadEvent.isReady()) {

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
    namedPipeThread = std::thread(
      listen, (void*)this
    );
    collectorThread = std::thread(
      close_tracked_connections,
      stopCollectorEvent,
      &connectionsQueue,
      (void*)this);
  }

  void on_quit()
  {

    /*
    The order of the operations in this function is extremely important,
    changing it could result in the component not closing, while waiting
    for its spawned threads to be closed.

    The operations we are doing in order to ensure a safe stop are:
    - mark the named pipe thread to stop, by activatings its event
    - connect to the underlying address as a final client, in order
      to force the accept() method to return.
    - close the actual named pipe thread.
    - push the non-closed connections to the collecting thread, so that
      they will be closed.
    - wait for the spawned threads to be stopped.
    */
    listener.close();
    stopNamedPipeThreadEvent.set();

    auto clientHandle = connect_client_to_pipe(std::string(RPC_ADDRESS), 200);
    if (clientHandle) CloseHandle((*clientHandle));
    namedPipeThread.join();

    // Track the non-closed connections for closing them up.
    std::for_each(
      trackedConnections.begin(),
      trackedConnections.end(),
      [&](PipeConnection conn) { connectionsQueue.push(conn); });
    connectionsQueue.push(_dummyConnection);

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