#pragma once
#include <map>
#include <string>

#include "rpc/playback_control.h"
#include "rpc/coreversion.h"
#include "rpc/playlist.h"
#include "factory.h"

using namespace std;
using namespace serialization;

namespace foobar {
  typedef map<string, std::function<Payload(vector<char> &)>> dispatch_map;

  class MethodDispatcher {
  private:    
    RpcPlaybackControl rpc_playback_control;
    RpcCoreVersion rpc_core_version;
    RpcPlaylist rpc_playlist;
    dispatch_map registry;
    
  public:

    Payload dispatch(vector<char> received) {
      string method_name;
      vector<char> buf;

      auto dst = serializer.unpack<tuple<string, vector<char>>>(received);
      tie(method_name, buf) = dst;

      dispatch_map::const_iterator iter = registry.find(method_name);
      if (iter == registry.end())
      {
        throw RPCException("Cannot find the given method.");
      }
      return iter->second(buf);
    }

    MethodDispatcher() {

      registry["CoreVersion.get_version_string"] = [&](vector<char> & param) {
        return rpc_core_version.get_version_string(param);
      };

      registry["CoreVersion.get_name"] = [&](vector<char> & param) {
        return rpc_core_version.get_name(param);
      };

      registry["CoreVersion.get_version"] = [&](vector<char> & param) {
        return rpc_core_version.get_version(param);
      };

      registry["CoreVersion.get_version_as_text"] = [&](vector<char> & param) {
        return rpc_core_version.get_version_as_text(param);
      };

      registry["CoreVersion.test_version"] = [&](vector<char> & param) {
        return rpc_core_version.test_version(param);
      };

      registry["Playlist.get_playlist_count"] = [&](vector<char> & param) {
        return rpc_playlist.get_playlist_count(param);
      };

      registry["Playlist.get_active_playlist"] = [&](vector<char> & param) {
        return rpc_playlist.get_active_playlist(param);
      };

      registry["Playlist.set_active_playlist"] = [&](vector<char> & param) {
        return rpc_playlist.set_active_playlist(param);
      };

      registry["Playlist.get_playing_playlist"] = [&](vector<char> & param) {
        return rpc_playlist.get_playing_playlist(param);
      };

      registry["Playlist.set_playing_playlist"] = [&](vector<char> & param) {
        return rpc_playlist.set_playing_playlist(param);
      };

      registry["Playlist.remove_playlists"] = [&](vector<char> & param) {
        return rpc_playlist.remove_playlists(param);
      };

      registry["Playlist.create_playlist"] = [&](vector<char> & param) {
        return rpc_playlist.create_playlist(param);
      };

      registry["Playlist.reorder"] = [&](vector<char> & param) {
        return rpc_playlist.reorder(param);
      };

      registry["Playlist.playlist_get_item_count"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_get_item_count(param);
      };

      registry["Playlist.playlist_get_focus_item"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_get_focus_item(param);
      };

      registry["Playlist.playlist_get_name"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_get_name(param);
      };

      registry["Playlist.playlist_reorder_items"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_reorder_items(param);
      };

      registry["Playlist.activeplaylist_reorder_items"] = [&](vector<char> & param) {
        return rpc_playlist.activeplaylist_reorder_items(param);
      };

      registry["Playlist.activeplaylist_set_selection"] = [&](vector<char> & param) {
        return rpc_playlist.activeplaylist_set_selection(param);
      };

      registry["Playlist.playlist_set_selection"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_set_selection(param);
      };

      registry["Playlist.activeplaylist_remove_items"] = [&](vector<char> & param) {
        return rpc_playlist.activeplaylist_remove_items(param);
      };

      registry["Playlist.playlist_remove_items"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_remove_items(param);
      };

      registry["Playlist.activeplaylist_replace_item"] = [&](vector<char> & param) {
        return rpc_playlist.activeplaylist_replace_item(param);
      };

      registry["Playlist.playlist_replace_item"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_replace_item(param);
      };

      registry["Playlist.playlist_set_focus_item"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_set_focus_item(param);
      };

      registry["Playlist.activeplaylist_set_focus_item"] = [&](vector<char> & param) {
        return rpc_playlist.activeplaylist_set_focus_item(param);
      };

      registry["Playlist.activeplaylist_insert_items"] = [&](vector<char> & param) {
        return rpc_playlist.activeplaylist_insert_items(param);
      };

      registry["Playlist.activeplaylist_add_items"] = [&](vector<char> & param) {
        return rpc_playlist.activeplaylist_add_items(param);
      };

      registry["Playlist.playlist_add_items"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_add_items(param);
      };

      registry["Playlist.playlist_insert_items"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_insert_items(param);
      };

      registry["Playlist.activeplaylist_ensure_visible"] = [&](vector<char> & param) {
        return rpc_playlist.activeplaylist_ensure_visible(param);
      };

      registry["Playlist.playlist_ensure_visible"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_ensure_visible(param);
      };

      registry["Playlist.activeplaylist_rename"] = [&](vector<char> & param) {
        return rpc_playlist.activeplaylist_rename(param);
      };

      registry["Playlist.playlist_rename"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_rename(param);
      };

      registry["Playlist.activeplaylist_undo_backup"] = [&](vector<char> & param) {
        return rpc_playlist.activeplaylist_undo_backup(param);
      };

      registry["Playlist.playlist_undo_backup"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_undo_backup(param);
      };

      registry["Playlist.activeplaylist_undo_restore"] = [&](vector<char> & param) {
        return rpc_playlist.activeplaylist_undo_restore(param);
      };

      registry["Playlist.playlist_undo_restore"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_undo_restore(param);
      };

      registry["Playlist.activeplaylist_redo_restore"] = [&](vector<char> & param) {
        return rpc_playlist.activeplaylist_redo_restore(param);
      };

      registry["Playlist.playlist_redo_restore"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_redo_restore(param);
      };

      registry["Playlist.activeplaylist_is_undo_available"] = [&](vector<char> & param) {
        return rpc_playlist.activeplaylist_is_undo_available(param);
      };

      registry["Playlist.playlist_is_undo_available"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_is_undo_available(param);
      };

      registry["Playlist.activeplaylist_is_redo_available"] = [&](vector<char> & param) {
        return rpc_playlist.activeplaylist_is_redo_available(param);
      };

      registry["Playlist.playlist_is_redo_available"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_is_redo_available(param);
      };

      registry["Playlist.activeplaylist_item_format_title"] = [&](vector<char> & param) {
        return rpc_playlist.activeplaylist_item_format_title(param);
      };

      registry["Playlist.playlist_item_format_title"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_item_format_title(param);
      };

      registry["Playlist.get_playing_item_location"] = [&](vector<char> & param) {
        return rpc_playlist.get_playing_item_location(param);
      };

      registry["Playlist.activeplaylist_sort_by_format"] = [&](vector<char> & param) {
        return rpc_playlist.activeplaylist_sort_by_format(param);
      };

      registry["Playlist.playlist_sort_by_format"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_sort_by_format(param);
      };

      registry["Playlist.playback_order_get_count"] = [&](vector<char> & param) {
        return rpc_playlist.playback_order_get_count(param);
      };

      registry["Playlist.playback_order_get_name"] = [&](vector<char> & param) {
        return rpc_playlist.playback_order_get_name(param);
      };

      registry["Playlist.playback_order_get_active"] = [&](vector<char> & param) {
        return rpc_playlist.playback_order_get_active(param);
      };

      registry["Playlist.playback_order_set_active"] = [&](vector<char> & param) {
        return rpc_playlist.playback_order_set_active(param);
      };

      registry["Playlist.queue_add_item_playlist"] = [&](vector<char> & param) {
        return rpc_playlist.queue_add_item_playlist(param);
      };

      registry["Playlist.queue_add_item"] = [&](vector<char> & param) {
        return rpc_playlist.queue_add_item(param);
      };

      registry["Playlist.queue_get_count"] = [&](vector<char> & param) {
        return rpc_playlist.queue_get_count(param);
      };

      registry["Playlist.queue_get_contents"] = [&](vector<char> & param) {
        return rpc_playlist.queue_get_contents(param);
      };

      registry["Playlist.queue_remove_mask"] = [&](vector<char> & param) {
        return rpc_playlist.queue_remove_mask(param);
      };

      registry["Playlist.queue_flush"] = [&](vector<char> & param) {
        return rpc_playlist.queue_flush(param);
      };

      registry["Playlist.queue_is_active"] = [&](vector<char> & param) {
        return rpc_playlist.queue_is_active(param);
      };

      registry["Playlist.highlight_playing_item"] = [&](vector<char> & param) {
        return rpc_playlist.highlight_playing_item(param);
      };

      registry["Playlist.remove_playlist"] = [&](vector<char> & param) {
        return rpc_playlist.remove_playlist(param);
      };

      registry["Playlist.remove_playlist_switch"] = [&](vector<char> & param) {
        return rpc_playlist.remove_playlist_switch(param);
      };

      registry["Playlist.activeplaylist_is_item_selected"] = [&](vector<char> & param) {
        return rpc_playlist.activeplaylist_is_item_selected(param);
      };

      registry["Playlist.playlist_is_item_selected"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_is_item_selected(param);
      };

      registry["Playlist.activeplaylist_move_selection"] = [&](vector<char> & param) {
        return rpc_playlist.activeplaylist_move_selection(param);
      };

      registry["Playlist.playlist_move_selection"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_move_selection(param);
      };

      registry["Playlist.activeplaylist_clear"] = [&](vector<char> & param) {
        return rpc_playlist.activeplaylist_clear(param);
      };

      registry["Playlist.playlist_clear"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_clear(param);
      };

      registry["Playlist.activeplaylist_clear_selection"] = [&](vector<char> & param) {
        return rpc_playlist.activeplaylist_clear_selection(param);
      };

      registry["Playlist.playlist_clear_selection"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_clear_selection(param);
      };

      registry["Playlist.activeplaylist_remove_selection"] = [&](vector<char> & param) {
        return rpc_playlist.activeplaylist_remove_selection(param);
      };

      registry["Playlist.playlist_remove_selection"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_remove_selection(param);
      };

      registry["Playlist.active_playlist_get_name"] = [&](vector<char> & param) {
        return rpc_playlist.active_playlist_get_name(param);
      };

      registry["Playlist.activeplaylist_get_item_count"] = [&](vector<char> & param) {
        return rpc_playlist.activeplaylist_get_item_count(param);
      };

      registry["Playlist.activeplaylist_get_focus_item"] = [&](vector<char> & param) {
        return rpc_playlist.activeplaylist_get_focus_item(param);
      };

      registry["Playlist.create_playlist_autoname"] = [&](vector<char> & param) {
        return rpc_playlist.create_playlist_autoname(param);
      };

      registry["Playlist.reset_playing_playlist"] = [&](vector<char> & param) {
        return rpc_playlist.reset_playing_playlist(param);
      };

      registry["Playlist.find_playlist"] = [&](vector<char> & param) {
        return rpc_playlist.find_playlist(param);
      };

      registry["Playlist.find_or_playlist"] = [&](vector<char> & param) {
        return rpc_playlist.find_or_playlist(param);
      };

      registry["Playlist.find_or_create_playlist_unlocked"] = [&](vector<char> & param) {
        return rpc_playlist.find_or_create_playlist_unlocked(param);
      };

      registry["Playlist.active_playlist_fix"] = [&](vector<char> & param) {
        return rpc_playlist.active_playlist_fix(param);
      };

      registry["Playlist.playlist_activate_delta"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_activate_delta(param);
      };

      registry["Playlist.playlist_activate_next"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_activate_next(param);
      };

      registry["Playlist.playlist_activate_previous"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_activate_previous(param);
      };

      registry["Playlist.playlist_get_selection_count"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_get_selection_count(param);
      };

      registry["Playlist.activeplaylist_get_selection_count"] = [&](vector<char> & param) {
        return rpc_playlist.activeplaylist_get_selection_count(param);
      };

      registry["Playlist.playlist_set_selection_single"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_set_selection_single(param);
      };

      registry["Playlist.activeplaylist_set_selection_single"] = [&](vector<char> & param) {
        return rpc_playlist.activeplaylist_set_selection_single(param);
      };

      registry["Playlist.activeplaylist_get_all_items"] = [&](vector<char> & param) {
        return rpc_playlist.activeplaylist_get_all_items(param);
      };

      registry["Playlist.playlist_get_all_items"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_get_all_items(param);
      };

      registry["Playlist.activeplaylist_get_selected_items"] = [&](vector<char> & param) {
        return rpc_playlist.activeplaylist_get_selected_items(param);
      };

      registry["Playlist.playlist_get_selected_items"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_get_selected_items(param);
      };

      registry["Playlist.activeplaylist_get_item_handle"] = [&](vector<char> & param) {
        return rpc_playlist.activeplaylist_get_item_handle(param);
      };

      registry["Playlist.playlist_get_item_handle"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_get_item_handle(param);
      };

      registry["Playlist.playlist_get_focus_item_handle"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_get_focus_item_handle(param);
      };

      registry["Playlist.activeplaylist_get_focus_item_handle"] = [&](vector<char> & param) {
        return rpc_playlist.activeplaylist_get_focus_item_handle(param);
      };

      registry["Playlist.activeplaylist_get_items"] = [&](vector<char> & param) {
        return rpc_playlist.activeplaylist_get_items(param);
      };

      registry["Playlist.playlist_get_items"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_get_items(param);
      };

      registry["Playlist.playlist_insert_items_filter"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_insert_items_filter(param);
      };


      registry["PlaybackControl.get_now_playing"] = [&](vector<char> & param) {
        return rpc_playback_control.get_now_playing(param);
      };

      registry["PlaybackControl.start"] = [&](vector<char> & param) {
        return rpc_playback_control.start(param);
      };

      registry["PlaybackControl.stop"] = [&](vector<char> & param) {
        return rpc_playback_control.stop(param);
      };

      registry["PlaybackControl.is_playing"] = [&](vector<char> & param) {
        return rpc_playback_control.is_playing(param);
      };

      registry["PlaybackControl.is_paused"] = [&](vector<char> & param) {
        return rpc_playback_control.is_paused(param);
      };

      registry["PlaybackControl.pause"] = [&](vector<char> & param) {
        return rpc_playback_control.pause(param);
      };

      registry["PlaybackControl.get_stop_after_current"] = [&](vector<char> & param) {
        return rpc_playback_control.get_stop_after_current(param);
      };

      registry["PlaybackControl.set_stop_after_current"] = [&](vector<char> & param) {
        return rpc_playback_control.set_stop_after_current(param);
      };

      registry["PlaybackControl.set_volume"] = [&](vector<char> & param) {
        return rpc_playback_control.set_volume(param);
      };

      registry["PlaybackControl.get_volume"] = [&](vector<char> & param) {
        return rpc_playback_control.get_volume(param);
      };

      registry["PlaybackControl.volume_up"] = [&](vector<char> & param) {
        return rpc_playback_control.volume_up(param);
      };

      registry["PlaybackControl.volume_down"] = [&](vector<char> & param) {
        return rpc_playback_control.volume_down(param);
      };

      registry["PlaybackControl.volume_mute_toggle"] = [&](vector<char> & param) {
        return rpc_playback_control.volume_mute_toggle(param);
      };

      registry["PlaybackControl.playback_seek"] = [&](vector<char> & param) {
        return rpc_playback_control.playback_seek(param);
      };

      registry["PlaybackControl.playback_seek_delta"] = [&](vector<char> & param) {
        return rpc_playback_control.playback_seek_delta(param);
      };

      registry["PlaybackControl.playback_can_seek"] = [&](vector<char> & param) {
        return rpc_playback_control.playback_can_seek(param);
      };

      registry["PlaybackControl.playback_get_position"] = [&](vector<char> & param) {
        return rpc_playback_control.playback_get_position(param);
      };

      registry["PlaybackControl.playback_format_title"] = [&](vector<char> & param) {
        return rpc_playback_control.playback_format_title(param);
      };

      registry["PlaybackControl.playback_format_title_complete"] = [&](vector<char> & param) {
        return rpc_playback_control.playback_format_title_complete(param);
      };

      registry["PlaybackControl.playback_get_length"] = [&](vector<char> & param) {
        return rpc_playback_control.playback_get_length(param);
      };

      registry["PlaybackControl.playback_get_length_ex"] = [&](vector<char> & param) {
        return rpc_playback_control.playback_get_length_ex(param);
      };

      registry["PlaybackControl.toggle_stop_after_current"] = [&](vector<char> & param) {
        return rpc_playback_control.toggle_stop_after_current(param);
      };

      registry["PlaybackControl.toggle_pause"] = [&](vector<char> & param) {
        return rpc_playback_control.toggle_pause(param);
      };

      registry["PlaybackControl.play_or_pause"] = [&](vector<char> & param) {
        return rpc_playback_control.play_or_pause(param);
      };

      registry["PlaybackControl.play_or_unpause"] = [&](vector<char> & param) {
        return rpc_playback_control.play_or_unpause(param);
      };

      registry["PlaybackControl.previous"] = [&](vector<char> & param) {
        return rpc_playback_control.previous(param);
      };

      registry["PlaybackControl.next"] = [&](vector<char> & param) {
        return rpc_playback_control.next(param);
      };

      registry["PlaybackControl.is_muted"] = [&](vector<char> & param) {
        return rpc_playback_control.is_muted(param);
      };

      registry["PlaybackControl.get_volume_step"] = [&](vector<char> & param) {
        return rpc_playback_control.get_volume_step(param);
      };


    }

  };

}
