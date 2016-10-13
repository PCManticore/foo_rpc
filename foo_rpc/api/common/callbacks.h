#pragma once

#include <vector>

using namespace std;

#include "../../stdafx.h"
#include "track.h"
#include "optional.hpp"

using namespace std::experimental;

// Callback used for getting a list all the items from a playlist.
class enum_items_callback_all : public playlist_manager::enum_items_callback
{
public:
  vector<Track> m_out;

  enum_items_callback_all() {}

  bool on_item(t_size p_index, const metadb_handle_ptr & p_location, bool b_selected)
  {
    m_out.push_back(Track::newTrack(p_index, p_location, b_selected));
    return true;
  }
};

class enum_items_callback_retrieve_item_opt : public playlist_manager::enum_items_callback
{
  optional<Track> track;

public:

  bool on_item(t_size p_index, const metadb_handle_ptr & p_location, bool b_selected)
  {
    track = optional<Track>(Track::newTrack(p_index, p_location, b_selected));
    return false;
  }

  inline const optional<Track> & get_item() {
    return track;
  }

};