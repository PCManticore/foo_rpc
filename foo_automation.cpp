#include <string>
#include <deque>

#include "logging.h"
#include "_winapi.h"
#include "stdafx.h"

using namespace std;


/*
- events for stopping a created thread
- one thread for having the named pipe server
- another thread for having the parsing server that pushes into foobar queue
- the foobar api implementation
- add logging when something fails
*/


DECLARE_COMPONENT_VERSION(
"foo_automation",
"0.0.1",
"Sample foobar2000 component.\n"
"See http://yirkha.fud.cz/progs/foobar2000/tutorial.php"
);

#define CONNECTING_STATE 0 
#define READING_STATE 1 
#define WRITING_STATE 2
#define BUFSIZE 4096


class PipeListener {
public:
	PipeListener(string address) : pipeAddress(address) {
		createNamedPipe(true);
	}

	OverlappedObject * accept() {

		HANDLE handle;
		DWORD connectResult;
		DWORD lastError;
		BOOL success;

		createNamedPipe(false);
		handle = handles.front();
		handles.pop_front();


		OverlappedObject* overlapped = connect_pipe(handle);
		int res = wait_overlapped_event(overlapped);
		// TODO handle exceptional case

		tie(success, ignore, lastError) = get_overlapped_event(overlapped);
		if (!success) {
			logToFoobarConsole("Getting overlapped event failed with %d.", lastError);
			return NULL;
		}
		return overlapped;
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
		if (result == SUCCESS) {
			handles.push_back(pipe);
		}
		return result;
	}

};

class test : public initquit {
private:
	DWORD ThreadID;
	HANDLE Event = CreateEvent(NULL, FALSE, FALSE, NULL);
public:	

	DWORD create_named_pipe() {
		char buffer[1000] = "";
		DWORD res;
		char stringbuffer;
		// TODO: get this from a config file
		PipeListener listener("\\\\.\\pipe\\foobar2000");
		while (true) {
			logToFoobarConsole("acception new client %d", 2222);
			OverlappedObject * overlapped = listener.accept();
			logToFoobarConsole("client accepted");

			logToFoobarConsole("try to recv bytes from him");
			res = recv_bytes(overlapped->handle, buffer);
			logToFoobarConsole("finished recving bytes from him");
			logToFoobarConsole(string(buffer));
			
			//wstring bob(buffer);
			//string bill(bob.begin(), bob.end());
			//logToFoobarConsole(bill);

		}

		/*
			logToFoobarConsole("Start reading from the pipe.");
			while (ReadFile(pipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				
				buffer[dwRead] = '\0';

				
				logToFoobarConsole(string(buffer));
			}
		
		DisconnectNamedPipe(pipe);
		*/
		return 0;

	}

	static DWORD WINAPI StaticThreadStart(void* Param)
	{
		test* This = (test*)Param;
		return This->ThreadStart();
	}

	DWORD ThreadStart()
	{
		
		//while (true) {
		//	static_api_ptr_t<main_thread_callback_manager>()->add_callback(new service_impl_t<main_thread_callback_myimpl>());
		//	Sleep(4);
		//}
		return create_named_pipe();
	}

	void startMyThread()
	{		
		CreateThread(NULL, 0, StaticThreadStart, (void*) this, 0, &ThreadID);
	}

	virtual void on_init()
	{
		console::formatter() << "Ola" << core_version_info::g_get_version_string() << ".\n";

		// Get play_control service
		static_api_ptr_t<playback_control> pc;
		// Then call its method to get the information we need
		console::formatter() << "Volume: " << pfc::format_float(pc->get_volume(), 0, 2) << " dB";

		// Similarly with a different service and function
		static_api_ptr_t<playlist_manager> pm;
		console::formatter() << "Playlist count: " << pm->get_playlist_count();

		// A simple call - this will start playback immediately after startup
		pc->start();

		startMyThread();
	}

	virtual void on_quit()
	{

	}
};


initquit_factory_t<test> g_foo;

// The module attribute is specified in order to implement DllMain,
// DllRegisterServer and DllUnregisterServer
[module(dll, name = "MyServer", helpstring = "MyServer 1.0 Type Library")];
[emitidl];