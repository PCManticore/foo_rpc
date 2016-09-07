#pragma once
#include <string>
#include <map>
#include <tuple>

#include "stdafx.h"
#include "event.h"
#include "percolate.h"
#include "api/playback_control.h"
#include "serialization/msgpack.h"

using namespace std;
using namespace serialization;


namespace foobar {


  class RpcPlaybackControl {
  private:
    foobar::PlaybackControl pc;

  public:
    RpcPlaybackControl() {}

    Payload playback_format_title_complete(vector<char> & _unused) {      
      ApiResult<string> result;
      fb2k::inMainThread([&] {
        pc.playback_format_title_complete(result);
      });
      result.wait();
      
      return serialization::Msgpack::packed_result(result);
    }
  };
}

