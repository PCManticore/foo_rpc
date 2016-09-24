#pragma once

using namespace std;

class BaseException : public exception {
public:
  BaseException(const char* message) : m(message) {}
  BaseException(std::string message) : m(message) {}

  virtual ~BaseException() throw() {}

  virtual const char* what() const throw() {
    return m.c_str();
  }

protected:
  std::string m;

private:
  BaseException();
};


class RPCException : public BaseException {
public:
  using BaseException::BaseException;
};
class PipeException : public BaseException {
public:
  using BaseException::BaseException;
};
