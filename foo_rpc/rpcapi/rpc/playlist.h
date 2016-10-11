// Generated on 2016-15-18 01:09:15
#pragma once
#include <string>
#include <map>
#include <tuple>

#include "../../stdafx.h"
#include "../../event.h"
#include "../../percolate.h"
#include "../api/playlist.h"
#include "../factory.h"
#include "optional.hpp"

using namespace std;
using namespace std::experimental;
using namespace serialization;


class RpcPlaylist {
  private:
    foobar::Playlist api;
  public:

    Payload get_playlist_count(vector<char> & buffer) {
      ApiResult<t_size> result;
      fb2k::inMainThread([&] {
        api.get_playlist_count(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }

    Payload get_active_playlist(vector<char> & buffer) {
      ApiResult<t_size> result;
      fb2k::inMainThread([&] {
        api.get_active_playlist(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }

    Payload set_active_playlist(vector<char> & buffer) {
      ApiParam<t_size> param(serialization::serializer.unpack<t_size>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.set_active_playlist(param, event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload get_playing_playlist(vector<char> & buffer) {
      ApiResult<t_size> result;
      fb2k::inMainThread([&] {
        api.get_playing_playlist(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }

    Payload set_playing_playlist(vector<char> & buffer) {
      ApiParam<t_size> param(serialization::serializer.unpack<t_size>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.set_playing_playlist(param, event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload remove_playlists(vector<char> & buffer) {
      ApiParam<vector<t_size> > param(serialization::serializer.unpack<vector<t_size>>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.remove_playlists(param, event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload create_playlist(vector<char> & buffer) {
      ApiParam<tuple<string, t_size, t_size> > param(serialization::serializer.unpack<tuple<string, t_size, t_size>>(buffer));
      ApiResult<t_size> result;
      fb2k::inMainThread([&] {
        api.create_playlist(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload create_playlist_ex(vector<char> & buffer) {
      ApiParam<tuple<string, t_size, t_size, vector<string> > > param(serialization::serializer.unpack<tuple<string, t_size, t_size, vector<string> >>(buffer));
      ApiResult<t_size> result;
      fb2k::inMainThread([&] {
        api.create_playlist_ex(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload reorder(vector<char> & buffer) {
      ApiParam<vector<int> > param(serialization::serializer.unpack<vector<int>>(buffer));
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.reorder(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload playlist_get_item_count(vector<char> & buffer) {
      ApiParam<t_size> param(serialization::serializer.unpack<t_size>(buffer));
      ApiResult<t_size> result;
      fb2k::inMainThread([&] {
        api.playlist_get_item_count(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload playlist_get_focus_item(vector<char> & buffer) {
      ApiParam<t_size> param(serialization::serializer.unpack<t_size>(buffer));
      ApiResult<t_size> result;
      fb2k::inMainThread([&] {
        api.playlist_get_focus_item(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload playlist_get_name(vector<char> & buffer) {
      ApiParam<t_size> param(serialization::serializer.unpack<t_size>(buffer));
      ApiResult<pfc::string8> result;
      fb2k::inMainThread([&] {
        api.playlist_get_name(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload playlist_reorder_items(vector<char> & buffer) {
      ApiParam<tuple<t_size, vector<int> > > param(serialization::serializer.unpack<tuple<t_size, vector<int> >>(buffer));
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.playlist_reorder_items(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload activeplaylist_reorder_items(vector<char> & buffer) {
      ApiParam<vector<int> > param(serialization::serializer.unpack<vector<int>>(buffer));
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.activeplaylist_reorder_items(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload activeplaylist_set_selection(vector<char> & buffer) {
      ApiParam<tuple<vector<t_size>, vector<bool> > > param(serialization::serializer.unpack<tuple<vector<t_size>, vector<bool> >>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.activeplaylist_set_selection(param, event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload playlist_set_selection(vector<char> & buffer) {
      ApiParam<tuple<t_size, vector<t_size>, vector<bool> > > param(serialization::serializer.unpack<tuple<t_size, vector<t_size>, vector<bool> >>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.playlist_set_selection(param, event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload activeplaylist_remove_items(vector<char> & buffer) {
      ApiParam<vector<t_size> > param(serialization::serializer.unpack<vector<t_size>>(buffer));
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.activeplaylist_remove_items(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload playlist_remove_items(vector<char> & buffer) {
      ApiParam<tuple<t_size, vector<t_size> > > param(serialization::serializer.unpack<tuple<t_size, vector<t_size> >>(buffer));
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.playlist_remove_items(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload activeplaylist_replace_item(vector<char> & buffer) {
      ApiParam<tuple<t_size, string> > param(serialization::serializer.unpack<tuple<t_size, string>>(buffer));
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.activeplaylist_replace_item(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload playlist_replace_item(vector<char> & buffer) {
      ApiParam<tuple<t_size, t_size, string> > param(serialization::serializer.unpack<tuple<t_size, t_size, string>>(buffer));
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.playlist_replace_item(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload playlist_set_focus_item(vector<char> & buffer) {
      ApiParam<tuple<t_size, t_size> > param(serialization::serializer.unpack<tuple<t_size, t_size>>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.playlist_set_focus_item(param, event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload activeplaylist_set_focus_item(vector<char> & buffer) {
      ApiParam<t_size> param(serialization::serializer.unpack<t_size>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.activeplaylist_set_focus_item(param, event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload activeplaylist_insert_items(vector<char> & buffer) {
      ApiParam<tuple<t_size, vector<string> > > param(serialization::serializer.unpack<tuple<t_size, vector<string> >>(buffer));
      ApiResult<t_size> result;
      fb2k::inMainThread([&] {
        api.activeplaylist_insert_items(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload activeplaylist_add_items(vector<char> & buffer) {
      ApiParam<vector<string> > param(serialization::serializer.unpack<vector<string>>(buffer));
      ApiResult<t_size> result;
      fb2k::inMainThread([&] {
        api.activeplaylist_add_items(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload playlist_add_items(vector<char> & buffer) {
      ApiParam<tuple<t_size, vector<string> > > param(serialization::serializer.unpack<tuple<t_size, vector<string> >>(buffer));
      ApiResult<t_size> result;
      fb2k::inMainThread([&] {
        api.playlist_add_items(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload playlist_insert_items(vector<char> & buffer) {
      ApiParam<tuple<t_size, t_size, vector<string> > > param(serialization::serializer.unpack<tuple<t_size, t_size, vector<string> >>(buffer));
      ApiResult<t_size> result;
      fb2k::inMainThread([&] {
        api.playlist_insert_items(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload activeplaylist_ensure_visible(vector<char> & buffer) {
      ApiParam<t_size> param(serialization::serializer.unpack<t_size>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.activeplaylist_ensure_visible(param, event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload playlist_ensure_visible(vector<char> & buffer) {
      ApiParam<tuple<t_size, t_size> > param(serialization::serializer.unpack<tuple<t_size, t_size>>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.playlist_ensure_visible(param, event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload activeplaylist_rename(vector<char> & buffer) {
      ApiParam<tuple<string, t_size>> param(
        serialization::serializer.unpack<tuple<string, t_size>>(buffer));
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.activeplaylist_rename(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload playlist_rename(vector<char> & buffer) {
      ApiParam<tuple<t_size, string, t_size>> param(
        serialization::serializer.unpack<tuple<t_size, string, t_size>>(buffer));
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.playlist_rename(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload activeplaylist_undo_backup(vector<char> & _unused) {
      Event event;
      fb2k::inMainThread([&] {
        api.activeplaylist_undo_backup(event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload playlist_undo_backup(vector<char> & buffer) {
      ApiParam<t_size> param(serialization::serializer.unpack<t_size>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.playlist_undo_backup(param, event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload activeplaylist_undo_restore(vector<char> & buffer) {
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.activeplaylist_undo_restore(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }

    Payload playlist_undo_restore(vector<char> & buffer) {
      ApiParam<t_size> param(serialization::serializer.unpack<t_size>(buffer));
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.playlist_undo_restore(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload activeplaylist_redo_restore(vector<char> & buffer) {
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.activeplaylist_redo_restore(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }

    Payload playlist_redo_restore(vector<char> & buffer) {
      ApiParam<t_size> param(serialization::serializer.unpack<t_size>(buffer));
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.playlist_redo_restore(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload activeplaylist_is_undo_available(vector<char> & buffer) {
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.activeplaylist_is_undo_available(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }

    Payload playlist_is_undo_available(vector<char> & buffer) {
      ApiParam<t_size> param(serialization::serializer.unpack<t_size>(buffer));
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.playlist_is_undo_available(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload activeplaylist_is_redo_available(vector<char> & buffer) {
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.activeplaylist_is_redo_available(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }

    Payload playlist_is_redo_available(vector<char> & buffer) {
      ApiParam<t_size> param(serialization::serializer.unpack<t_size>(buffer));
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.playlist_is_redo_available(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload activeplaylist_item_format_title(vector<char> & buffer) {
      ApiParam<tuple<t_size, string> > param(serialization::serializer.unpack<tuple<t_size, string>>(buffer));
      ApiResult<pfc::string8> result;
      fb2k::inMainThread([&] {
        api.activeplaylist_item_format_title(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload playlist_item_format_title(vector<char> & buffer) {
      ApiParam<tuple<t_size, t_size, string> > param(serialization::serializer.unpack<tuple<t_size, t_size, string>>(buffer));
      ApiResult<pfc::string8> result;
      fb2k::inMainThread([&] {
        api.playlist_item_format_title(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload get_playing_item_location(vector<char> & buffer) {
      ApiResult<tuple<bool, t_size, t_size> > result;
      fb2k::inMainThread([&] {
        api.get_playing_item_location(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }

    Payload activeplaylist_sort_by_format(vector<char> & buffer) {
      ApiParam<tuple<string, bool> > param(serialization::serializer.unpack<tuple<string, bool>>(buffer));
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.activeplaylist_sort_by_format(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload playlist_sort_by_format(vector<char> & buffer) {
      ApiParam<tuple<t_size, string, bool> > param(serialization::serializer.unpack<tuple<t_size, string, bool>>(buffer));
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.playlist_sort_by_format(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload playback_order_get_count(vector<char> & buffer) {
      ApiResult<t_size> result;
      fb2k::inMainThread([&] {
        api.playback_order_get_count(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }

    Payload playback_order_get_name(vector<char> & buffer) {
      ApiParam<t_size> param(serialization::serializer.unpack<t_size>(buffer));
      ApiResult<string> result;
      fb2k::inMainThread([&] {
        api.playback_order_get_name(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload playback_order_get_active(vector<char> & buffer) {
      ApiResult<t_size> result;
      fb2k::inMainThread([&] {
        api.playback_order_get_active(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }

    Payload playback_order_set_active(vector<char> & buffer) {
      ApiParam<t_size> param(serialization::serializer.unpack<t_size>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.playback_order_set_active(param, event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload queue_add_item_playlist(vector<char> & buffer) {
      ApiParam<tuple<t_size, t_size> > param(serialization::serializer.unpack<tuple<t_size, t_size>>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.queue_add_item_playlist(param, event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload queue_add_item(vector<char> & buffer) {
      ApiParam<string> param(serialization::serializer.unpack<string>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.queue_add_item(param, event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload queue_get_count(vector<char> & buffer) {
      ApiResult<t_size> result;
      fb2k::inMainThread([&] {
        api.queue_get_count(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }

    Payload queue_get_contents(vector<char> & buffer) {
      ApiResult<vector<t_playback_queue_item> > result;
      fb2k::inMainThread([&] {
        api.queue_get_contents(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }

    Payload queue_remove_mask(vector<char> & buffer) {
      ApiParam<vector<t_size> > param(serialization::serializer.unpack<vector<t_size>>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.queue_remove_mask(param, event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload queue_flush(vector<char> & _unused) {
      Event event;
      fb2k::inMainThread([&] {
        api.queue_flush(event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload queue_is_active(vector<char> & buffer) {
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.queue_is_active(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }

    Payload highlight_playing_item(vector<char> & buffer) {
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.highlight_playing_item(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }

    Payload remove_playlist(vector<char> & buffer) {
      ApiParam<t_size> param(serialization::serializer.unpack<t_size>(buffer));
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.remove_playlist(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload remove_playlist_switch(vector<char> & buffer) {
      ApiParam<t_size> param(serialization::serializer.unpack<t_size>(buffer));
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.remove_playlist_switch(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload activeplaylist_is_item_selected(vector<char> & buffer) {
      ApiParam<t_size> param(serialization::serializer.unpack<t_size>(buffer));
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.activeplaylist_is_item_selected(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload playlist_is_item_selected(vector<char> & buffer) {
      ApiParam<tuple<t_size, t_size> > param(serialization::serializer.unpack<tuple<t_size, t_size>>(buffer));
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.playlist_is_item_selected(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload activeplaylist_move_selection(vector<char> & buffer) {
      ApiParam<int> param(serialization::serializer.unpack<int>(buffer));
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.activeplaylist_move_selection(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload playlist_move_selection(vector<char> & buffer) {
      ApiParam<tuple<t_size, int> > param(serialization::serializer.unpack<tuple<t_size, int>>(buffer));
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.playlist_move_selection(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload activeplaylist_clear(vector<char> & _unused) {
      Event event;
      fb2k::inMainThread([&] {
        api.activeplaylist_clear(event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload playlist_clear(vector<char> & buffer) {
      ApiParam<t_size> param(serialization::serializer.unpack<t_size>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.playlist_clear(param, event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload activeplaylist_clear_selection(vector<char> & _unused) {
      Event event;
      fb2k::inMainThread([&] {
        api.activeplaylist_clear_selection(event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload playlist_clear_selection(vector<char> & buffer) {
      ApiParam<t_size> param(serialization::serializer.unpack<t_size>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.playlist_clear_selection(param, event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload activeplaylist_remove_selection(vector<char> & buffer) {
      ApiParam<bool> param(serialization::serializer.unpack<bool>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.activeplaylist_remove_selection(param, event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload playlist_remove_selection(vector<char> & buffer) {
      ApiParam<tuple<t_size, bool> > param(serialization::serializer.unpack<tuple<t_size, bool>>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.playlist_remove_selection(param, event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload activeplaylist_get_name(vector<char> & buffer) {
      ApiResult<pfc::string8> result;
      fb2k::inMainThread([&] {
        api.activeplaylist_get_name(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }

    Payload activeplaylist_get_item_count(vector<char> & buffer) {
      ApiResult<t_size> result;
      fb2k::inMainThread([&] {
        api.activeplaylist_get_item_count(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }

    Payload activeplaylist_get_focus_item(vector<char> & buffer) {
      ApiResult<t_size> result;
      fb2k::inMainThread([&] {
        api.activeplaylist_get_focus_item(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }

    Payload create_playlist_autoname(vector<char> & buffer) {
      ApiParam<t_size> param(serialization::serializer.unpack<t_size>(buffer));
      ApiResult<t_size> result;
      fb2k::inMainThread([&] {
        api.create_playlist_autoname(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload reset_playing_playlist(vector<char> & _unused) {
      Event event;
      fb2k::inMainThread([&] {
        api.reset_playing_playlist(event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload find_playlist(vector<char> & buffer) {
      ApiParam<tuple<string, t_size> > param(serialization::serializer.unpack<tuple<string, t_size>>(buffer));
      ApiResult<t_size> result;
      fb2k::inMainThread([&] {
        api.find_playlist(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload find_or_create_playlist(vector<char> & buffer) {
      ApiParam<tuple<string, t_size> > param(serialization::serializer.unpack<tuple<string, t_size>>(buffer));
      ApiResult<t_size> result;
      fb2k::inMainThread([&] {
        api.find_or_create_playlist(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload find_or_create_playlist_unlocked(vector<char> & buffer) {
      ApiParam<tuple<string, t_size> > param(serialization::serializer.unpack<tuple<string, t_size>>(buffer));
      ApiResult<t_size> result;
      fb2k::inMainThread([&] {
        api.find_or_create_playlist_unlocked(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload active_playlist_fix(vector<char> & _unused) {
      Event event;
      fb2k::inMainThread([&] {
        api.active_playlist_fix(event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload playlist_activate_delta(vector<char> & buffer) {
      ApiParam<int> param(serialization::serializer.unpack<int>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.playlist_activate_delta(param, event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload playlist_activate_next(vector<char> & _unused) {
      Event event;
      fb2k::inMainThread([&] {
        api.playlist_activate_next(event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload playlist_activate_previous(vector<char> & _unused) {
      Event event;
      fb2k::inMainThread([&] {
        api.playlist_activate_previous(event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload playlist_get_selection_count(vector<char> & buffer) {
      ApiParam<tuple<t_size, t_size> > param(serialization::serializer.unpack<tuple<t_size, t_size>>(buffer));
      ApiResult<t_size> result;
      fb2k::inMainThread([&] {
        api.playlist_get_selection_count(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload activeplaylist_get_selection_count(vector<char> & buffer) {
      ApiParam<t_size> param(serialization::serializer.unpack<t_size>(buffer));
      ApiResult<t_size> result;
      fb2k::inMainThread([&] {
        api.activeplaylist_get_selection_count(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload playlist_set_selection_single(vector<char> & buffer) {
      ApiParam<tuple<t_size, t_size, bool> > param(serialization::serializer.unpack<tuple<t_size, t_size, bool>>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.playlist_set_selection_single(param, event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload activeplaylist_set_selection_single(vector<char> & buffer) {
      ApiParam<tuple<t_size, bool> > param(serialization::serializer.unpack<tuple<t_size, bool>>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.activeplaylist_set_selection_single(param, event);
      });
      event.wait();
      return serialization::serializer.packed(true);
    }

    Payload activeplaylist_get_all_items(vector<char> & buffer) {
      ApiResult<vector<Track> > result;
      fb2k::inMainThread([&] {
        api.activeplaylist_get_all_items(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }

    Payload playlist_get_all_items(vector<char> & buffer) {
      ApiParam<t_size> param(serialization::serializer.unpack<t_size>(buffer));
      ApiResult<vector<Track> > result;
      fb2k::inMainThread([&] {
        api.playlist_get_all_items(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload activeplaylist_get_selected_items(vector<char> & buffer) {
      ApiResult<vector<Track> > result;
      fb2k::inMainThread([&] {
        api.activeplaylist_get_selected_items(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }

    Payload playlist_get_selected_items(vector<char> & buffer) {
      ApiParam<t_size> param(serialization::serializer.unpack<t_size>(buffer));
      ApiResult<vector<Track> > result;
      fb2k::inMainThread([&] {
        api.playlist_get_selected_items(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload activeplaylist_get_item_handle(vector<char> & buffer) {
      ApiParam<t_size> param(serialization::serializer.unpack<t_size>(buffer));
      ApiResult<optional<Track>> result;
      fb2k::inMainThread([&] {
        api.activeplaylist_get_item_handle(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload playlist_get_item_handle(vector<char> & buffer) {
      ApiParam<tuple<t_size, t_size> > param(serialization::serializer.unpack<tuple<t_size, t_size>>(buffer));
      ApiResult<std::experimental::optional<Track>> result;
      fb2k::inMainThread([&] {
        api.playlist_get_item_handle(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload playlist_get_focus_item_handle(vector<char> & buffer) {
      ApiParam<t_size> param(serialization::serializer.unpack<t_size>(buffer));
      ApiResult<optional<Track>> result;
      fb2k::inMainThread([&] {
        api.playlist_get_focus_item_handle(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload activeplaylist_get_focus_item_handle(vector<char> & buffer) {
      ApiResult<optional<Track>> result;
      fb2k::inMainThread([&] {
        api.activeplaylist_get_focus_item_handle(result);
      });
      result.wait();            
      return serialization::serializer.packed(result);
    }

    Payload activeplaylist_get_items(vector<char> & buffer) {
      ApiParam<vector<t_size> > param(serialization::serializer.unpack<vector<t_size>>(buffer));
      ApiResult<vector<Track> > result;
      fb2k::inMainThread([&] {
        api.activeplaylist_get_items(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload playlist_get_items(vector<char> & buffer) {
      ApiParam<tuple<t_size, vector<t_size> > > param(serialization::serializer.unpack<tuple<t_size, vector<t_size> >>(buffer));
      ApiResult<vector<Track> > result;
      fb2k::inMainThread([&] {
        api.playlist_get_items(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }

    Payload playlist_insert_items_filter(vector<char> & buffer) {
      ApiParam<tuple<t_size, t_size, bool, vector<string> > > param(serialization::serializer.unpack<tuple<t_size, t_size, bool, vector<string> >>(buffer));
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.playlist_insert_items_filter(param, result);
      });
      result.wait();
      return serialization::serializer.packed(result);
    }
};