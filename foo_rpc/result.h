#pragma once

#define SENTINEL_ERROR -9999

template<class T>
class Result {
private:
  T obj;
  DWORD errorCode = SENTINEL_ERROR;

  Result() {}

public:

  static Result<T> withError(DWORD code) {
    Result<T> instance = Result();
    instance.errorCode = code;
    return instance;
  };

  Result(T t) : obj(t) {}

  DWORD isFailed() {
    return this->errorCode != SENTINEL_ERROR;
  }

  DWORD error() {
    return this->errorCode;
  }

  T result() {
    return this->obj;
  }
};


