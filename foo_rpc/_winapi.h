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
  OVERLAPPED overlapped;
  DWORD pending;
  DWORD completed;
} OverlappedObject, *LPOverlappedObject;


OverlappedObject* new_overlapped(HANDLE handle);
DWORD wait_overlapped_event(OverlappedObject* overlapped);
DWORD create_pipe(std::string pipeAddress, HANDLE * pipeOut, bool isFirst);
Result<DWORD> send_bytes(HANDLE handle, string writeBuffer, int len);
Result<tuple<DWORD, DWORD>> recv_bytes(HANDLE handle, char * readBuffer, int size);
Result<OverlappedObject*> connect_pipe(HANDLE handle);
Result<DWORD> get_overlapped_result(OverlappedObject * overlapped);
Result<tuple<DWORD, vector<char>>> get_more_data(HANDLE handle);

