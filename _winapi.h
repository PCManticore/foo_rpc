#pragma once
#include <tuple>
#include <string>
#include <windows.h>

#include "result.h"

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
DWORD wait_overlapped_event(OverlappedObject* overlapped);
DWORD create_pipe(std::string pipeAddress, HANDLE * pipeOut, bool isFirst);
Result<DWORD> recv_bytes(HANDLE handle, char * readBuffer, int size);
Result<DWORD> send_bytes(HANDLE handle, const char * writeBuffer, int len);
Result<OverlappedObject*> connect_pipe(HANDLE handle);
Result<DWORD> get_overlapped_event(OverlappedObject * overlapped);
Result<tuple<DWORD, DWORD>> peek_named_pipe(HANDLE handle, char * buf, int size);

