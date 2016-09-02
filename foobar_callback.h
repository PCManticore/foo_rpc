#pragma once
#include "stdafx.h"
#include "event.h"

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

  class current_playlist_callback : public main_thread_callback {
  public:
    void callback_run() {
      pc->next();
      refObj.set();
    }

    current_playlist_callback(Event obj) : refObj(obj) {}
  private:
    Event refObj;
    static_api_ptr_t<playback_control> pc;
  };
};
