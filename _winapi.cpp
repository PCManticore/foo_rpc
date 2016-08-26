#pragma once
#include <assert.h>
#include <string>
#include "_winapi.h"
#include "logging.h"

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

DWORD create_pipe(std::string pipeAddress,
	              bool first,
	              HANDLE* pipeOut) {

	DWORD flags = PIPE_ACCESS_DUPLEX | FILE_FLAG_OVERLAPPED;
	if (first) {
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
		// TODO: pass GetLastError
		logToFoobarConsole("Failed creating named pipe.");
		return GetLastError();
	}
	return CREATE_NAMED_PIPE_SUCCESS;
}

OverlappedObject* connect_pipe(HANDLE handle) {
	DWORD success;

	OverlappedObject *overlapped = new_overlapped(handle);
	success = ConnectNamedPipe(
		handle,
		overlapped ? &overlapped->overlapped : NULL);

	if (!overlapped) {
		logToFoobarConsole("Cannot create an overlapped object.");
		return 0;
	}

	int err = GetLastError();
	/* Overlapped ConnectNamedPipe never returns a success code */
	assert(success == 0);
	if (err == ERROR_IO_PENDING) {
		logToFoobarConsole("Operation pending.");
		overlapped->pending = 1;
	}
	else if (err == ERROR_PIPE_CONNECTED) {
		logToFoobarConsole("Pipe connected.");
		SetEvent(overlapped->overlapped.hEvent);
	}
	else {
		logToFoobarConsole("Could not connect to the named pipe.");
		// TODO: log this with stderr
		return 0;
	}
	return overlapped;
}
