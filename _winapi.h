#pragma once
#include <tuple>
#include <string>
#include <windows.h>

using namespace std;

#define CONNECTING_STATE 0 
#define READING_STATE 1 
#define WRITING_STATE 2
#define BUFSIZE 4096
#define SUCCESS 1
#define FAILED 0

typedef struct {

	HANDLE handle;
	HANDLE event;
	OVERLAPPED overlapped;
	DWORD pending;
	DWORD completed;
} OverlappedObject, *LPOverlappedObject;


OverlappedObject* new_overlapped(HANDLE handle);
OverlappedObject* connect_pipe(HANDLE handle);
DWORD wait_overlapped_event(OverlappedObject* overlapped);
DWORD recv_bytes(HANDLE handle, char * readBuffer);
DWORD create_pipe(std::string pipeAddress, HANDLE* pipeOut, bool isFirst);
tuple<DWORD, DWORD, DWORD> get_overlapped_event(OverlappedObject * overlapped);

