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


  class RpcPlaybackControl {
  private:
    foobar::PlaybackControl pc;

  public:
    RpcPlaybackControl() {}

    string playback_format_title_complete(vector<char> & _unused) {

      ApiResult<tuple<string, BOOL>> result;

      fb2k::inMainThread([&] {
        pc.playback_format_title_complete(result);
      });
      result.wait();

      msgpack::sbuffer sbuf;
      tuple<string, bool> converted = result.result();
      // TODO: maybe we should drop BOOL altogether?
      msgpack::pack(sbuf, converted);

      int resSize = sbuf.size();
      string str(sbuf.data());
      return str.substr(0, resSize);
    }
  };
}

