#pragma once

#include <string>
#include <vector>

#include "track.h"
#include "callbacks.h"
#include "../../stdafx.h"
#include "optional.hpp"

using namespace std::experimental;

optional<Track> get_item_as_track(t_size p_item, t_size p_playlist) {
  static_api_ptr_t<playlist_manager> manager;

  enum_items_callback_retrieve_item_opt callback;
  manager->playlist_enum_items(
    p_playlist, callback, bit_array_one(p_item));

  return callback.get_item();
}

pfc::list_t<metadb_handle_ptr> metadb_handles_from_files(std::vector<string> files) {
  static_api_ptr_t<metadb> metadb_manager;

  pfc::list_t<metadb_handle_ptr> handles;
  for (auto file : files) {
    metadb_handle_ptr handle;
    metadb_manager->handle_create(handle, make_playable_location(file.c_str(), 0));
    handles.add_item(handle);
  }
  return handles;
}
