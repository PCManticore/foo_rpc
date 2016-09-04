#pragma once
#include <string>

#include "api/debug.h"

#include "tinyformat.h"


template<typename ...Args>
void logToFoobarConsole(std::string message, const Args&... args) {
  std::ostringstream oss;
  std::string formattedMessage;
  const char* fmt_c;

  fmt_c = message.c_str();
  tfm::format(oss, fmt_c, args...);
  formattedMessage = oss.str();
  
  static_api_ptr_t<main_thread_callback_manager>()->add_callback(
    new service_impl_t<foobar::console_debug_callback>(formattedMessage)
  );
}
