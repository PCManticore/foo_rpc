#pragma once

#include "base.h"
#include "../../logging.h"
#include "../../local_exceptions.h"

#include "msgpack.hpp"

MSGPACK_ADD_ENUM(play_control::t_track_command);

namespace serialization {  
 
  class Msgpack : public Serializer {
  private:
    template<typename T>
    Payload packed_generic(T data) {
      msgpack::sbuffer sbuf;      
      msgpack::pack(sbuf, data);
      // This is quite trashy, but we will lose the buffer
      // once it goes out of scope.
      Payload payload(string(sbuf.data()), sbuf.size());
      return payload;
    }    

  public:
    template<typename T>
    T unpack(vector<char> buf) {
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
    
    Payload packed_bool(bool value) {
      return packed_generic(to_string(value));
    };

    template<typename T>
    Payload packed_result(ApiResult<T> result) {
      return packed_generic(result.result());
    }
  };
};