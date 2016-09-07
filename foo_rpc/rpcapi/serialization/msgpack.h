#pragma once

#include "base.h"
#include "../../logging.h"
#include "../../local_exceptions.h"

#include "msgpack.hpp"

namespace serialization {  

  class Msgpack : Serializer {
  public:
    template<typename T>
    static T unpack(vector<char> buf) {
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
    };

    template<typename T>
    static Payload packed_result(ApiResult<T> result) {
      msgpack::sbuffer sbuf;
      T converted = result.result();
      msgpack::pack(sbuf, converted);
      // This is quite trashy, but we will lose the buffer
      // once it goes out of scope.
      Payload payload(string(sbuf.data()), sbuf.size());
      return payload;
    }; 
  
  };
};