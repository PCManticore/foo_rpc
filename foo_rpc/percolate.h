#pragma once

#include <windows.h>

#include "event.h"

template<class T>
class ApiParam {
private:
  T obj;

public:

  ApiParam(T value) : obj(value) {}

  T value() {
    return obj;
  }
};


template<class T>
class ApiResult {
private:
  Event event;  
  T resultObject;

public:


  void setResult(T value) {
    this->resultObject = value;
    event.set();
  }

  bool isReady() {
    return event.isReady();
  }

  bool wait() {
    return event.wait();
  }

  T result() {
    return this->resultObject;
  }

};
