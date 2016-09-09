#pragma once

#include "base.h"
#include "../../logging.h"
#include "../../local_exceptions.h"

#include "msgpack.hpp"

MSGPACK_ADD_ENUM(play_control::t_track_command);

namespace msgpack {
  MSGPACK_API_VERSION_NAMESPACE(MSGPACK_DEFAULT_API_NS) {
    namespace adaptor {


      template<>
      struct pack<pfc::string8> {
        template <typename Stream>
        packer<Stream>& operator()(msgpack::packer<Stream>& o, pfc::string8 const& v) const {
          o.pack(v.c_str());
          return o;
        }
      };
    }
  }
}

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