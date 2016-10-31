#pragma once
#include <windows.h>

class Event {
private:
  HANDLE event = CreateEvent(NULL, TRUE, FALSE, NULL);

public:
  Event() {}

  Event(const Event& other) : event(other.event) {}

  Event& operator=(const Event& other) {
    event = other.event;
    return *this;
  }

  void set() { SetEvent(event); }

  bool isReady() {
    DWORD result = WaitForSingleObject(event, 0);
    return (result == WAIT_OBJECT_0);
  }

  bool wait() {
    DWORD result = WaitForSingleObject(event, INFINITE);
    return (result == WAIT_OBJECT_0);
  }
};

