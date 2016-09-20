#pragma once
#include <string>

#include "../stdafx.h"
#include "../event.h"
#include "../percolate.h"
#include "common/track.h"

using namespace std;

namespace foobar {

  class PlaybackControl {
  private:
    static_api_ptr_t<playback_control> pc;
    static_api_ptr_t<playback_control_v2> pc2;
    static_api_ptr_t<playback_control_v3> pc3;

  public:
    PlaybackControl() {}
    
    void get_now_playing(ApiResult<tuple<bool, OptionalTrack>> & result) {
      static_api_ptr_t<playlist_manager> pm;
      t_size p_playlist, p_index;        
      bool success = pm->get_playing_item_location(&p_playlist, &p_index);
      if (success) {
        metadb_handle_ptr now_playing = pm->playlist_get_item_handle(p_playlist, p_index);
        
        auto track = Track::newTrack(
          p_index,
          now_playing,
          pm->playlist_is_item_selected(p_playlist, p_index));        
        result.setResult(make_tuple(success, OptionalTrack(track)));
      }
      else {
        result.setResult(make_tuple(false, OptionalTrack()));
      }
    }

    void start(ApiParam<tuple<play_control::t_track_command, bool>> param, Event event) {
      play_control::t_track_command command;
      bool is_paused;
      tie(command, is_paused) = param.value();

      pc->start(command, is_paused);
      event.set();
    }

    void stop(Event event) {
      pc->stop();
      event.set();
    }

    void is_playing(ApiResult<bool> & response) {
      bool isPlaying = pc->is_playing();
      response.setResult(isPlaying);
    }

    void is_paused(ApiResult<bool> & response) {
      bool isPaused = pc->is_paused();
      response.setResult(isPaused);
    }

    void pause(ApiParam<bool> param, Event event) {
      pc->pause(param.value());
      event.set();
    }

    void get_stop_after_current(ApiResult<bool> & response) {
      bool stopAfterCurrent = pc->get_stop_after_current();
      response.setResult(stopAfterCurrent);
    }

    void set_stop_after_current(ApiParam<bool> param, Event event) {
      pc->set_stop_after_current(param.value());
      event.set();
    }

    void set_volume(ApiParam<float> volume, Event event) {
      pc->set_volume(volume.value());
      event.set();
    }

    void get_volume(ApiResult<float> & response) {
      float volume = pc->get_volume();
      response.setResult(volume);
    }

    void volume_up(Event event) {
      pc->volume_up();
      event.set();
    }

    void volume_down(Event event) {
      pc->volume_down();
      event.set();
    }

    void volume_mute_toggle(Event event) {
      pc->volume_mute_toggle();
      event.set();
    }

    void playback_seek(ApiParam<double> param, Event event) {
      pc->playback_seek(param.value());
      event.set();
    }

    void playback_seek_delta(ApiParam<double> param, Event event) {
      pc->playback_seek_delta(param.value());
      event.set();
    }

    void playback_can_seek(ApiResult<bool> & response) {
      bool canSeek = pc->playback_can_seek();
      response.setResult(canSeek);
    }

    void playback_get_position(ApiResult<double> & response) {
      double position = pc->playback_get_position();
      response.setResult(position);
    }

    void playback_format_title(ApiParam<string> param, ApiResult<string> & response) {

      pfc::string8 temp;      
      titleformat_object::ptr script;
      static_api_ptr_t<titleformat_compiler>()->compile_force(script, param.value().c_str());
      
      bool success = pc->playback_format_title(
        NULL, temp, script, NULL,
        playback_control::t_display_level::display_level_all);
      if (success) {
        response.setResult(temp.c_str());
      }
      else {
        response.setResult("");
      }
    }

    void playback_format_title_complete(ApiResult<string> & response) {
      ApiParam<string> param(
        "[%album artist% -]['['%album%[CD%discnumber%]"
        "[#%tracknumber%]']'] % title%['//' %track artist%]");

      playback_format_title(param, response);
    }

    void playback_get_length(ApiResult<double> & response) {
      double length = pc->playback_get_length();
      response.setResult(length);
    }

    void playback_get_length_ex(ApiResult<double> & response) {
      double length = pc->playback_get_length_ex();
      response.setResult(length);
    }

    void toggle_stop_after_current(Event event) {
      pc->toggle_stop_after_current();
      event.set();
    }

    void toggle_pause(Event event) {
      pc->toggle_pause();
      event.set();
    }

    void play_or_pause(Event event) {
      pc->play_or_pause();
      event.set();
    }

    void play_or_unpause(Event event) {
      pc->play_or_unpause();
      event.set();
    }

    void previous(Event event) {
      pc->previous();
      event.set();
    }

    void next(Event event) {
      pc->next();
      event.set();
    }

    void is_muted(ApiResult<bool> & response) {
      bool isMuted = pc->is_muted();
      response.setResult(isMuted);
    }

    void get_volume_step(ApiResult<float> & response) {
      float volume_step = pc2->get_volume_step();
      response.setResult(volume_step);
    }

  };
}
