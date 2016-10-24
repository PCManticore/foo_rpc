#pragma once

#include "base.h"
#include "../../logging.h"
#include "../../local_exceptions.h"
#include "../../api/common/track.h"
#include "../rpctype.h"
#include "optional.hpp"
#include "msgpack.hpp"

using namespace std::experimental;

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

      template<>
      struct pack<t_playback_queue_item> {
        template <typename Stream>
        packer<Stream>& operator()(msgpack::packer<Stream>& o, t_playback_queue_item const& v) const {
          map<string, t_size> playback_item;
          playback_item["item"] = v.m_item;
          playback_item["playlist"] = v.m_playlist;
          o.pack(playback_item);
          return o;
        }
      };

      template<>
      struct pack<Track> {
        template <typename Stream>
        packer<Stream>& operator()(msgpack::packer<Stream>& o, Track const& v) const {
          o.pack_array(4);
          o.pack(make_tuple("index", v.index));
          o.pack(make_tuple("path", v.path));
          o.pack(make_tuple("selected", v.selected));
          o.pack(make_tuple("subsong_index", v.subsong_index));
          return o;
        }
      };

      template<>
      struct pack<optional<Track>> {
        template <typename Stream>
        packer<Stream>& operator()(msgpack::packer<Stream>& o, optional<Track> const& v) const {
          if (v) {
            Track track = *v;
            o.pack(track);
          }
          else {
            o.pack_nil();
          }
          return o;          
        }
      };

        template<>
        struct pack<optional<string>> {
          template <typename Stream>
          packer<Stream>& operator()(msgpack::packer<Stream>& o, optional<string> const& v) const {
            if (v) {
              auto value = *v;
              o.pack(value);
            }
            else {
              o.pack_nil();
            }
            return o;
          };
        };
    };
  };
};


namespace serialization {  
 
  class Msgpack : public Serializer {
  private:
    template<typename T>
    Payload packed_generic(T data) {
      msgpack::sbuffer sbuf;      
      msgpack::pack(sbuf, data);
      // This is quite trashy, but we will lose the buffer
      // once it goes out of scope.
      Payload payload(string(sbuf.data(), sbuf.size()), sbuf.size());
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
        auto repr = repr_type<T>::value();
        auto exc = tfm::format("Failed unpacking, while expecting type %s", repr);
        logToFoobarConsole("Error while unpacking from buffer: %s", exc);
        throw RPCException(exc);
      }
    };

    Payload packed(tuple<int, bool> value) {
      return packed_generic(
        make_tuple(
          std::get<0>(value),
          to_string(std::get<1>(value))
        )
      );
    }

    Payload packed(tuple<int, const char*> result) {
      return packed_generic(result);
    }

    template<typename T>
    Payload packed(tuple<int, ApiResult<T>> result) {

      return packed_generic(make_tuple(
        std::get<0>(result),
        std::get<1>(result).result()
      ));
    }
    
  };
};