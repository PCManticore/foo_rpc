#pragma once
#include <string>
#include <map>
#include <tuple>

#include "stdafx.h"
#include "event.h"
#include "percolate.h"
#include "api/playback_control.h"
#include "factory.h"

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
            
      return serialization::serializer.packed_result(result);
    }

    Payload play_or_pause(vector<char> & _unused) {
      Event event;
      fb2k::inMainThread([&] {
        pc.play_or_pause(event);
      });
      event.wait();
      return serialization::serializer.packed_bool(true);
    }

    Payload set_volume(vector<char> & buffer) {
      ApiParam<float> param(serialization::serializer.unpack<float>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        pc.set_volume(param, event);
      });
      event.wait();
      return serialization::serializer.packed_bool(true);
    }

  };
}

