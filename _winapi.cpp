#pragma once
#include <assert.h>
#include <string>
#include <tuple>

#include "_winapi.h"
#include "logging.h"

using namespace std;

OverlappedObject *new_overlapped(HANDLE handle)
{
	OverlappedObject *self = new OverlappedObject();
	self->handle = handle;
	self->pending = 0;
	self->completed = 0;
	memset(&self->overlapped, 0, sizeof(OVERLAPPED));	
	/* Manual reset, initially non-signalled */
	self->overlapped.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
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
		return FAILED;
	}
	return SUCCESS;
}

OverlappedObject* connect_pipe(HANDLE handle) {
	DWORD success;

	OverlappedObject *overlapped = new_overlapped(handle);
	success = ConnectNamedPipe(
		handle,
		overlapped ? &overlapped->overlapped : NULL);

	if (!overlapped) {
		logToFoobarConsole(
			"Cannot create an overlapped object. "
		    "The operation failed with error %d.",
			GetLastError());
		return FAILED;
	}

	int err = GetLastError();
	/* Overlapped ConnectNamedPipe never returns a success code */
	assert(success == 0);
	if (err == ERROR_IO_PENDING) {
		// TODO HOW TO HANDLE THIS?
		overlapped->pending = 1;
	}
	else if (err == ERROR_PIPE_CONNECTED) {
		SetEvent(overlapped->overlapped.hEvent);
	}
	else {
		logToFoobarConsole("Could not connect to the named pipe. "
		                   "The operation failed with error %d.", err);
		// TODO: log this with stderr
		return FAILED;
	}
	return overlapped;
}

DWORD wait_overlapped_event(OverlappedObject* overlapped) {
	HANDLE events[1] = { overlapped->event };
	return WaitForMultipleObjects(1, events, FALSE, INFINITE);
}

tuple<DWORD, DWORD, DWORD> get_overlapped_event(OverlappedObject * overlapped) {
	DWORD result;
	DWORD error;
	DWORD transferred = 0;
	
	result = GetOverlappedResult(
		overlapped->handle,
		&overlapped->overlapped,
		&transferred,
		true);

	error = result ? ERROR_SUCCESS : GetLastError();	

	switch (error) {
	    case ERROR_SUCCESS:
	    case ERROR_MORE_DATA:
	    case ERROR_OPERATION_ABORTED:
		    overlapped->completed = 1;
		    overlapped->pending = 0;
		    break;
	    case ERROR_IO_INCOMPLETE:
		    break;
	    default:
		    overlapped->pending = 0;
		    return make_tuple(FAILED, result, error);
	}
	return make_tuple(SUCCESS, result, error);
}


DWORD read_from_pipe(HANDLE handle, int size,
	                 OverlappedObject * overlapped,
	                 char * readBuffer) {

	DWORD nread;
	DWORD err;
	BOOL ret;

	// TODO: don't return the same value (0 or ret, it might be confusing)
	overlapped = new_overlapped(handle);
	if (!overlapped) {
		logToFoobarConsole("Cannot read from pipe.");
		return FAILED;
	}
	
	ret = ReadFile(
		handle, readBuffer, size, &nread,
		overlapped ? &overlapped->overlapped : NULL);

	err = ret ? 0 : GetLastError();
	if (!ret) {
		if (err == ERROR_IO_PENDING)
			// TODO: what to do with this pending field?
			overlapped->pending = 1;
		else if (err != ERROR_MORE_DATA) {
			logToFoobarConsole("Cannot read from pipe. "
				                "The operation failed with error %d.", err);
			return FAILED;
		}
	}
	return SUCCESS;
}


DWORD recv_bytes(HANDLE handle, char * readBuffer) {
	DWORD res;
	DWORD waitres;
	DWORD overlappedResult;
	DWORD transferred;
	BOOL success;
	DWORD lastError;

	int size = 256;	
	OverlappedObject * overlapped = new_overlapped(handle);

	res = read_from_pipe(handle, size, overlapped, readBuffer);

	if (res == ERROR_IO_PENDING) {
		waitres = wait_overlapped_event(overlapped);
		assert(waitres == WAIT_OBJECT_0);
	}

	tie(success, overlappedResult, lastError) = get_overlapped_event(overlapped);
	// todo handle success as well
	switch (lastError) {
	    case 0:
			// todo WHAT IS THIS
			logToFoobarConsole(std::to_string(GetLastError()));
		    return 0;
		case ERROR_MORE_DATA:
			// TODO: implement this
			break;
		default:
			// TODO what is this?
			logToFoobarConsole("getting last error %d", GetLastError());
			return overlappedResult;
	}
	return res;

}
