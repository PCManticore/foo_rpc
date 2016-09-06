#pragma once

using namespace std;


class RPCException : public exception {
public:
  RPCException(const char* message) : m(message) {}

  virtual ~RPCException() throw() {}

  virtual const char* what() const throw() {
    return m.c_str();
  }

protected:
  std::string m;

private:
  RPCException();
};
