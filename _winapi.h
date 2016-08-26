#pragma once
#include <string>
#include <Windows.h>

#define CONNECTING_STATE 0 
#define READING_STATE 1 
#define WRITING_STATE 2
#define BUFSIZE 4096
#define CREATE_NAMED_PIPE_SUCCESS 1

typedef struct {

	HANDLE handle;
	DWORD pending;
	DWORD completed;
	HANDLE event;
	OVERLAPPED overlapped;
	TCHAR readBuffer[BUFSIZE];
	TCHAR writeBuffer[BUFSIZE];
} OverlappedObject, *LPOverlappedObject;


DWORD create_pipe(std::string pipeAddress, bool first, HANDLE* pipeOut);
OverlappedObject* new_overlapped(HANDLE handle);
OverlappedObject* connect_pipe(HANDLE handle);
DWORD wait_overlapped_event(OverlappedObject* overlapped);
DWORD recv_bytes(HANDLE handle, TCHAR * readBuffer);

