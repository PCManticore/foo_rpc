#pragma once
#include <string>
#include <map>
#include <tuple>

#include "stdafx.h"
#include "event.h"
#include "percolate.h"
#include "api/playback_control.h"

#include "msgpack_util.h"
#include "msgpack.hpp"


using namespace std;

namespace foobar {


  class RpcPlaylist {
  private:
    Playlist pl;

  public:
    RpcPlaylist() {}

    string playback_format_title_complete(vector<char> buf) {

      auto dst = unpack_from_buf<tuple<string, t_size, t_size>>(buf);
      ApiParam<tuple<const char*, t_size, t_size>> param(make_tuple(
        std::get<0>(dst).c_str(),
        std::get<1>(dst),
        std::get<2>(dst)
      ));
      ApiResult<t_size> resultAPI;

      fb2k::inMainThread([&] {
        pl.create_playlist(param, resultAPI);
      });
      resultAPI.wait();

      msgpack::sbuffer sbuf;
      msgpack::pack(sbuf, resultAPI.result());

      int resSize = sbuf.size();
      string str(sbuf.data());
      return str.substr(0, resSize);
    }
  };
}

namespace foobar {
  typedef map<string, std::function<string(vector<char> &)>> dispatch_map;

  class MethodDispatcher {
  private:
    RpcPlaylist rpc_playlist;
    dispatch_map registry;

  public:

    MethodDispatcher() {
      registry["Playlist.playback_format_title_complete"] = [&](vector<char> & param) {
        return rpc_playlist.playback_format_title_complete(param);
      };
    }

    string dispatch(vector<char> received) {
      string method_name;
      vector<char> buf;      

      auto dst = unpack_from_buf<tuple<string, vector<char>>>(received);
      tie(method_name, buf) = dst;

      dispatch_map::const_iterator iter = registry.find(method_name);
      if (iter == registry.end())
      {
        throw RPCException("Cannot find the given method.");
      }      
      return iter->second(buf);
    }

  };  

}
