#pragma once
#include "msgpack.hpp"
#include "../logging.h"
#include "../local_exceptions.h"

template<typename T>
T unpack_from_buf(vector<char> buf) {
  try {
    const char* bufdata = buf.data();
    msgpack::unpacked result;
    msgpack::unpack(result, bufdata, buf.size());
    msgpack::object obj = result.get();
    return obj.as<T>();
  }
  catch (msgpack::v1::type_error& e) {
    logToFoobarConsole("Error while unpacking from buffer: %s", e.what());
    throw RPCException(e.what());
  }

}