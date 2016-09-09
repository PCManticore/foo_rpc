// Generated on 2016-05-09 22:09:39
#pragma once
#include <string>
#include <map>
#include <tuple>

#include "stdafx.h"
#include "event.h"
#include "percolate.h"
#include "api/playlist.h"
#include "../factory.h"

using namespace std;
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
      return serialization::serializer.packed_result(result);
    }

    Payload get_active_playlist(vector<char> & buffer) {
      ApiResult<t_size> result;
      fb2k::inMainThread([&] {
        api.get_active_playlist(result);
      });
      result.wait();            
      return serialization::serializer.packed_result(result);
    }

    Payload set_active_playlist(vector<char> & buffer) {
      ApiParam<t_size> param(serialization::serializer.unpack<t_size>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.set_active_playlist(param, event);
      });
      event.wait();
      return serialization::serializer.packed_bool(true);
    }

    Payload get_playing_playlist(vector<char> & buffer) {
      ApiResult<t_size> result;
      fb2k::inMainThread([&] {
        api.get_playing_playlist(result);
      });
      result.wait();            
      return serialization::serializer.packed_result(result);
    }

    Payload set_playing_playlist(vector<char> & buffer) {
      ApiParam<t_size> param(serialization::serializer.unpack<t_size>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.set_playing_playlist(param, event);
      });
      event.wait();
      return serialization::serializer.packed_bool(true);
    }

    Payload remove_playlists(vector<char> & buffer) {
      ApiParam<vector<t_size> > param(serialization::serializer.unpack<vector<t_size>>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.remove_playlists(param, event);
      });
      event.wait();
      return serialization::serializer.packed_bool(true);
    }

    Payload create_playlist(vector<char> & buffer) {
      ApiParam< tuple<vector<char>, t_size, t_size> > param(serialization::serializer.unpack<tuple<vector<char>, t_size, t_size>>(buffer));
      ApiResult<t_size> result;
      fb2k::inMainThread([&] {
        api.create_playlist(param, result);
      });
      result.wait();
      return serialization::serializer.packed_result(result);
    }

    Payload reorder(vector<char> & buffer) {
      ApiParam<vector<int>> param(serialization::serializer.unpack<vector<int>>(buffer));
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.reorder(param, result);
      });
      result.wait();
      return serialization::serializer.packed_result(result);
    }

    Payload playlist_get_item_count(vector<char> & buffer) {
      ApiParam<t_size> param(serialization::serializer.unpack<t_size>(buffer));
      ApiResult<t_size> result;
      fb2k::inMainThread([&] {
        api.playlist_get_item_count(param, result);
      });
      result.wait();
      return serialization::serializer.packed_result(result);
    }

    Payload playlist_get_focus_item(vector<char> & buffer) {
      ApiParam<t_size> param(serialization::serializer.unpack<t_size>(buffer));
      ApiResult<t_size> result;
      fb2k::inMainThread([&] {
        api.playlist_get_focus_item(param, result);
      });
      result.wait();
      return serialization::serializer.packed_result(result);
    }

   Payload playlist_get_name(vector<char> & buffer) {
      ApiParam<t_size> param(serialization::serializer.unpack<t_size>(buffer));
      ApiResult<pfc::string8> result;
      fb2k::inMainThread([&] {
        api.playlist_get_name(param, result);
      });
      result.wait();
      return serialization::serializer.packed_result(result);
    }

    Payload playlist_reorder_items(vector<char> & buffer) {
      ApiParam<tuple<t_size, vector<int> > > param(serialization::serializer.unpack<tuple<t_size, vector<int> >>(buffer));
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.playlist_reorder_items(param, result);
      });
      result.wait();
      return serialization::serializer.packed_result(result);
    }

    Payload playlist_set_selection(vector<char> & buffer) {
      ApiParam<tuple<t_size, vector<t_size>, vector<bool> > > param(serialization::serializer.unpack<tuple<t_size, vector<t_size>, vector<bool> >>(buffer));
      Event event;
      fb2k::inMainThread([&] {
        api.playlist_set_selection(param, event);
      });
      event.wait();
      return serialization::serializer.packed_bool(true);
    }

    Payload playlist_remove_items(vector<char> & buffer) {
      ApiParam<tuple<t_size, vector<t_size> > > param(serialization::serializer.unpack<tuple<t_size, vector<t_size> >>(buffer));
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.playlist_remove_items(param, result);
      });
      result.wait();
      return serialization::serializer.packed_result(result);
    }
};