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

      registry["Playlist.playlist_set_selection"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_set_selection(param);
      };

      registry["Playlist.playlist_remove_items"] = [&](vector<char> & param) {
        return rpc_playlist.playlist_remove_items(param);
      };

    }

  };

}
