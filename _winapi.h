#pragma once
#include <tuple>
#include <string>
#include <windows.h>

#include "either.h"

using namespace std;

#define OPERATION_SUCCESS 1
#define OPERATION_FAILED 0

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
DWORD recv_bytes(HANDLE handle, char * readBuffer, int size);
DWORD create_pipe(std::string pipeAddress, HANDLE * pipeOut, bool isFirst);
//tuple<DWORD, DWORD, DWORD> get_overlapped_event(OverlappedObject * overlapped);
Maybe<tuple<DWORD, DWORD>> get_overlapped_event(OverlappedObject * overlapped);
Maybe<tuple<DWORD, DWORD>> peek_named_pipe(HANDLE handle, char * buf, int size);

