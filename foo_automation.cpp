#include <string>
#include <deque>

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

typedef struct{

	HANDLE handle;
	DWORD pending;
	DWORD completed;
	HANDLE event;
	OVERLAPPED overlapped;
	TCHAR readBuffer[BUFSIZE];
	TCHAR writeBuffer[BUFSIZE];
} OverlappedObject, *LPOverlappedObject;

namespace {
	class console_debug_callback : public main_thread_callback {
	  public:
		void callback_run() {
		  console::formatter() << debugMessage.c_str();
		}

		console_debug_callback(string message) : debugMessage(message) {}
 	  private:
		string debugMessage;
	};
}

/*Send a message to the Foobar console. */

void debugWithConsoleMessage(string message) {
	static_api_ptr_t<main_thread_callback_manager>()->add_callback(new service_impl_t<console_debug_callback>(message));
}



class PipeListener {
public:
	PipeListener(string address) : pipeAddress(address) {
		createNamedPipe(true);
	}

	OverlappedObject * accept() {

		HANDLE handle;
		DWORD connectResult;
		BOOL success;

		createNamedPipe(false);
		handle = handles.front();
		handles.pop_front();


		OverlappedObject* overlapped = connectNamedPipe(handle);
		HANDLE events[1];
		events[0] = overlapped->event;

		int res = WaitForMultipleObjects(1, events, FALSE, INFINITE);
		// TODO handle exceptional case

		DWORD transferred = 0;
		int res2 = GetOverlappedResult(overlapped->handle, &overlapped->overlapped,
			&transferred, true);
		int err = res2 ? ERROR_SUCCESS : GetLastError();
		switch (err) {
		case ERROR_SUCCESS:
		case ERROR_MORE_DATA:
		case ERROR_OPERATION_ABORTED:
			debugWithConsoleMessage("success");
			overlapped->completed = 1;
			overlapped->pending = 0;
			break;
		case ERROR_IO_INCOMPLETE:
			debugWithConsoleMessage("incomplete");
			break;
		default:
			overlapped->pending = 0;
			debugWithConsoleMessage("WTF?");
			return 0;
		}
		return overlapped;
	}

private:
	deque<HANDLE> handles = {};
	string pipeAddress;

	/*Creates a named pipe

	Returns 0 when the pipe couldn't be created, 1 otherwise.
	*/
	DWORD createNamedPipe(bool first) {
		DWORD flags = PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED;
		if (first) {
			flags |= FILE_FLAG_FIRST_PIPE_INSTANCE;
		}

		HANDLE pipe = CreateNamedPipe(
		    wstring(pipeAddress.begin(), pipeAddress.end()).c_str(),
			flags,
			PIPE_TYPE_MESSAGE | PIPE_READMODE_MESSAGE | PIPE_WAIT,
			PIPE_UNLIMITED_INSTANCES,
			1024 * 16,
			1024 * 16,
			NMPWAIT_WAIT_FOREVER,
			NULL);
		if (pipe == INVALID_HANDLE_VALUE) {
			// TODO: pass GetLastError
			debugWithConsoleMessage("Failed creating named pipe.");
			return GetLastError();
		}
		else {
			handles.push_back(pipe);
			return 1;
		}
	}

	OverlappedObject *new_overlapped(HANDLE handle)
	{
		OverlappedObject *self = new OverlappedObject();
		self->handle = handle;
		self->pending;
		self->completed;
		memset(&self->overlapped, 0, sizeof(OVERLAPPED));
		memset(&self->writeBuffer, 0, BUFSIZE);
		/* Manual reset, initially non-signalled */
		self->overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		return self;
	}

	OverlappedObject* connectNamedPipe(HANDLE handle) {
		DWORD success;

		OverlappedObject *overlapped = new_overlapped(handle);
		success = ConnectNamedPipe(
			handle,
			overlapped ? &overlapped->overlapped : NULL);

		if (!overlapped) {
			debugWithConsoleMessage("Cannot create an overlapped object.");
			return 0;
		}

		int err = GetLastError();
		/* Overlapped ConnectNamedPipe never returns a success code */
		assert(success == 0);
		if (err == ERROR_IO_PENDING) {
			debugWithConsoleMessage("Operation pending.");
			overlapped->pending = 1;
		}
		else if (err == ERROR_PIPE_CONNECTED) {
			debugWithConsoleMessage("Pipe connected.");
			SetEvent(overlapped->overlapped.hEvent);
		}
		else {
			debugWithConsoleMessage("Could not connect to the named pipe.");
			// TODO: log this with stderr
			return 0;
		}
		return overlapped;
	}
};

class test : public initquit {
private:
	DWORD ThreadID;
	HANDLE Event = CreateEvent(NULL, FALSE, FALSE, NULL);
public:	

	DWORD create_named_pipe() {
		char buffer[1024];
		// TODO: get this from a config file
		PipeListener listener("\\\\.\\pipe\\foobar2000");
		while (true) {
			OverlappedObject * overlapped = listener.accept();
		}

		/*
			debugWithConsoleMessage("Start reading from the pipe.");
			while (ReadFile(pipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
			{
				
				buffer[dwRead] = '\0';

				
				debugWithConsoleMessage(string(buffer));
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