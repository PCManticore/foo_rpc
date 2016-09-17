#pragma once

#include <vector>

using namespace std;

#include "../stdafx.h"
#include "track.h"

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

class enum_items_callback_retrieve_selected_items : public enum_items_callback_all
{
public:
  bool on_item(t_size p_index, const metadb_handle_ptr & p_location, bool b_selected)
  {
    if (b_selected) enum_items_callback_all::on_item(p_index, p_location, b_selected);
    return true;
  }
};

class enum_items_callback_retrieve_item : public playlist_manager::enum_items_callback
{
  vector<Track> m_out;

public:

  bool on_item(t_size p_index, const metadb_handle_ptr & p_location, bool b_selected)
  {
    m_out.push_back(Track::newTrack(p_index, p_location, b_selected));
    return false;
  }

  inline const vector<Track> & get_item() {
    return m_out;
  }

};
