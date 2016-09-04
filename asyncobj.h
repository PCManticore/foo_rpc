#pragma once

#include <windows.h>

#include "event.h"

template<class T>
class Param {
private:
  T obj;

public:

  Param(T value) : obj(value) {}

  T value() {
    return obj;
  }
};

// TODO: don't like the name Response, but cant use Result
template<class T>
class Response {
private:
  Event event;  
  T response;

public:


  void setResponse(T value) {
    this->response = value;
    event.set();
  }

  bool isReady() {
    return event.isReady();
  }

  bool wait() {
    return event.wait();
  }

  T result() {
    return this->response;
  }

};
