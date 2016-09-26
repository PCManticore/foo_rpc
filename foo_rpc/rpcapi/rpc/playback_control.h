// Generated on 2016-15-18 01:09:44
#pragma once
#include <string>
#include <map>
#include <tuple>

#include "../../stdafx.h"
#include "../../event.h"
#include "../../percolate.h"
#include "../../api/playback_control.h"
#include "../factory.h"
#include "optional.hpp"

using namespace std;
using namespace std::experimental;
using namespace serialization;


class RpcPlaybackControl {
  private:
    foobar::PlaybackControl api;
  public:

    Payload get_now_playing(vector<char> & buffer) {
      ApiResult<tuple<bool, optional<Track>>> result;
      fb2k::inMainThread([&] {
        api.get_now_playing(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }

    Payload start(vector<char> & buffer) {
      ApiParam<tuple<play_control::t_track_command, bool> > param(serialization::serializer.unpack<tuple<play_control::t_track_command, bool>>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.start(param, event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload stop(vector<char> & _unused) {
      Event event;
      fb2k::inMainThread([&] {
        api.stop(event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload is_playing(vector<char> & buffer) {
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.is_playing(result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload is_paused(vector<char> & buffer) {
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.is_paused(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }

    Payload pause(vector<char> & buffer) {
      ApiParam<bool> param(serialization::serializer.unpack<bool>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.pause(param, event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload get_stop_after_current(vector<char> & buffer) {
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.get_stop_after_current(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }

    Payload set_stop_after_current(vector<char> & buffer) {
      ApiParam<bool> param(serialization::serializer.unpack<bool>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.set_stop_after_current(param, event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload set_volume(vector<char> & buffer) {
      ApiParam<float> param(serialization::serializer.unpack<float>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.set_volume(param, event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload get_volume(vector<char> & buffer) {
      ApiResult<float> result;
      fb2k::inMainThread([&] {
        api.get_volume(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }

    Payload volume_up(vector<char> & _unused) {
      Event event;
      fb2k::inMainThread([&] {
        api.volume_up(event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload volume_down(vector<char> & _unused) {
      Event event;
      fb2k::inMainThread([&] {
        api.volume_down(event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload volume_mute_toggle(vector<char> & _unused) {
      Event event;
      fb2k::inMainThread([&] {
        api.volume_mute_toggle(event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload playback_seek(vector<char> & buffer) {
      ApiParam<double> param(serialization::serializer.unpack<double>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.playback_seek(param, event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload playback_seek_delta(vector<char> & buffer) {
      ApiParam<double> param(serialization::serializer.unpack<double>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.playback_seek_delta(param, event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload playback_can_seek(vector<char> & buffer) {
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.playback_can_seek(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }

    Payload playback_get_position(vector<char> & buffer) {
      ApiResult<double> result;
      fb2k::inMainThread([&] {
        api.playback_get_position(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }

    Payload playback_format_title(vector<char> & buffer) {
      ApiParam<string> param(serialization::serializer.unpack<string>(buffer));
      ApiResult<string> result;
      fb2k::inMainThread([&] {
        api.playback_format_title(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload playback_format_title_complete(vector<char> & buffer) {
      ApiResult<string> result;
      fb2k::inMainThread([&] {
        api.playback_format_title_complete(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }

    Payload playback_get_length(vector<char> & buffer) {
      ApiResult<double> result;
      fb2k::inMainThread([&] {
        api.playback_get_length(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }

    Payload playback_get_length_ex(vector<char> & buffer) {
      ApiResult<double> result;
      fb2k::inMainThread([&] {
        api.playback_get_length_ex(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }

    Payload toggle_stop_after_current(vector<char> & _unused) {
      Event event;
      fb2k::inMainThread([&] {
        api.toggle_stop_after_current(event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload toggle_pause(vector<char> & _unused) {
      Event event;
      fb2k::inMainThread([&] {
        api.toggle_pause(event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload play_or_pause(vector<char> & _unused) {
      Event event;
      fb2k::inMainThread([&] {
        api.play_or_pause(event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload play_or_unpause(vector<char> & _unused) {
      Event event;
      fb2k::inMainThread([&] {
        api.play_or_unpause(event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload previous(vector<char> & _unused) {
      Event event;
      fb2k::inMainThread([&] {
        api.previous(event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload next(vector<char> & _unused) {
      Event event;
      fb2k::inMainThread([&] {
        api.next(event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload is_muted(vector<char> & buffer) {
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.is_muted(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }

    Payload get_volume_step(vector<char> & buffer) {
      ApiResult<float> result;
      fb2k::inMainThread([&] {
        api.get_volume_step(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }
};