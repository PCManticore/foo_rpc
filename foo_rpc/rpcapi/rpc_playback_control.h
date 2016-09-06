#pragma once
#include <string>
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

    string playback_format_title_complete(const char* & buf) {

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
