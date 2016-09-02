#pragma once

#include <Windows.h>

#include "event.h"


template<class T>
class AsyncObj {
private:
  T obj;
  Event event;

public:

  void setValue(T value) {
    obj = value;
    event.set();
  }

  bool isReady() {
    return event.isReady();
  }

  bool wait() {
    return event.wait();
  }

  T value() {
    return obj;
  }

};
