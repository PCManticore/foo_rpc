#pragma once
#pragma once
#include <tuple>

#include "../stdafx.h"
#include "../percolate.h"
#include "common/helpers.h"
#include "optional.hpp"

using namespace std;
using namespace std::experimental;

namespace foobar {
  class MetadbHandle {
    static_api_ptr_t<playback_control> pc;

  public:
    void format_title(ApiParam<tuple<t_size, t_size, string>> param, ApiResult<optional<string>> & response) {
      string format;
      t_size playlist;
      t_size item;

      tie(playlist, item, format) = param.value();

      pfc::string8 temp;
      titleformat_object::ptr script;
      static_api_ptr_t<titleformat_compiler>()->compile_force(script, format.c_str());

      auto trackOpt = get_item_as_track(item, playlist);
      if (!trackOpt) {
        optional<string> empty;
        response.setResult(empty);
      } else {
        auto track = *trackOpt;
        auto success = track.handle->format_title(NULL, temp, script, NULL);

        if (success) {
          response.setResult(optional<string>(temp.c_str()));
        } else {
          optional<string> formattedTitle;
          response.setResult(formattedTitle);
        }
      }
    }
  };
};
