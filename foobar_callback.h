#pragma once
#include <string>

#include "stdafx.h"
#include "asyncobj.h"
#include "event.h"


using namespace std;

// TODO: merging this with the other one does not work (linker error)
namespace FoobarImpl {

  class PlaybackControl {
  private:
    static_api_ptr_t<playback_control> pc;
    static_api_ptr_t<playback_control_v2> pc2;
    static_api_ptr_t<playback_control_v3> pc3;

  public:
    PlaybackControl() {}
    
    // TODO: get_now_playing

    void get_now_playing() {
      metadb_handle_ptr temp;
      pc->get_now_playing(temp);      
    }

    void start(Param<tuple<play_control::t_track_command, BOOL>> param, Event event) {
      play_control::t_track_command command;
      BOOL is_paused;
      tie(command, is_paused) = param.value();

      pc->start(command, is_paused);
      event.set();
    }

    void stop(Event event) {
      pc->stop();
      event.set();
    }

    void is_playing(Response<bool> * response) {
      bool isPlaying = pc->is_playing();
      response->setResponse(isPlaying);
    }

    void is_paused(Response<bool> * response) {
      bool isPaused = pc->is_paused();
      response->setResponse(isPaused);
    }

    void pause(Param<bool> param, Event event) {
      pc->pause(param.value());
      event.set();
    }

    void get_stop_after_current(Response<bool> * response) {
      bool stopAfterCurrent = pc->get_stop_after_current();
      response->setResponse(stopAfterCurrent);
    }

    void set_stop_after_current(Param<bool> param, Event event) {
      pc->set_stop_after_current(param.value());
      event.set();
    }

    void set_volume(Param<float> volume, Event event) {
      pc->set_volume(volume.value());
      event.set();
    }

    void get_volume(Response<float> * response) {
      float volume = pc->get_volume();
      response->setResponse(volume);
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

    void playback_seek(Param<double> param, Event event) {
      pc->playback_seek(param.value());
      event.set();
    }

    void playback_seek_delta(Param<double> param, Event event) {
      pc->playback_seek_delta(param.value());
      event.set();
    }

    void playback_can_seek(Response<bool> * response) {
      bool canSeek = pc->playback_can_seek();
      response->setResponse(canSeek);
    }

    void playback_get_position(Response<double> * response) {
      double position = pc->playback_get_position();
      response->setResponse(position);
    }

    void playback_format_title(Param<string> param, Response<tuple<string, BOOL>> * response) {

      pfc::string8 temp;      
      titleformat_object::ptr script;
      static_api_ptr_t<titleformat_compiler>()->compile_force(script, param.value().c_str());
      
      bool success = pc->playback_format_title(
        NULL, temp, script, NULL,
        playback_control::t_display_level::display_level_all);
      if (success) {
        response->setResponse(make_tuple(temp.c_str(), success));
      }
      else {
        response->setResponse(make_tuple("", success));
      }
    }

    void playback_format_title_complete(Response<tuple<string, BOOL>> * response) {
      Param<string> param(
        "[%album artist% -]['['%album%[CD%discnumber%]"
        "[#%tracknumber%]']'] % title%['//' %track artist%]");

      playback_format_title(param, response);
    }

    void playback_get_length(Response<double> * response) {
      double length = pc->playback_get_length();
      response->setResponse(length);
    }

    void playback_get_length_ex(Response<double> * response) {
      double length = pc->playback_get_length_ex();
      response->setResponse(length);
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

    void is_muted(Response<bool> * response) {
      bool isMuted = pc->is_muted();
      response->setResponse(isMuted);
    }

    void get_volume_step(Response<float> * response) {
      float volume_step = pc2->get_volume_step();
      response->setResponse(volume_step);
    }

  };
}

namespace {

  class console_debug_callback : public main_thread_callback {
  public:
    void callback_run() {
      console::formatter() << debugMessage.c_str();
    }

    console_debug_callback(std::string message) : debugMessage(message) {}
  private:
    std::string debugMessage;
  };

};
