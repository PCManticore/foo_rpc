#pragma once
#include <map>
#include <vector>

#include "../stdafx.h"
#include "../percolate.h"

using namespace std;


namespace foobar {
  class Playlist {
  private:
    static_api_ptr_t<playlist_manager> playlist_manager;
    static_api_ptr_t<metadb> metadb_manager;
    
  public:
    void get_playlist_count(ApiResult<t_size> & result) {
      t_size count = playlist_manager->get_playlist_count();
      result.setResult(count);
    }

    void get_active_playlist(ApiResult<t_size> & result) {
      t_size activePlaylist = playlist_manager->get_active_playlist();
      result.setResult(activePlaylist);
    }

    void set_active_playlist(ApiParam<t_size> param, Event event) {
      t_size playlist = param.value();
      playlist_manager->set_active_playlist(playlist);
      event.set();
    }

    void get_playing_playlist(ApiResult<t_size> & result) {
      t_size playing_playlist = playlist_manager->get_playing_playlist();
      result.setResult(playing_playlist);
    }

    void set_playing_playlist(ApiParam<t_size> param, Event event) {
      t_size playling_playlist = param.value();
      playlist_manager->set_playing_playlist(playling_playlist);
      event.set();
    }

    void remove_playlists(ApiParam<vector<t_size>> param, Event event) {
      vector<t_size> elements = param.value();
      bit_array_bittable table(elements.size());
      for (auto elem : elements) {           
        table.set(elem, true);
      }

      playlist_manager->remove_playlists(table);
      event.set();
    }

    void create_playlist(ApiParam <tuple<vector<char>, t_size, t_size>> param,
                         ApiResult<t_size> & result) {
      vector<char> p_name;
      t_size p_name_length, p_index;
      tie(p_name, p_name_length, p_index) = param.value();

      t_size created_index = playlist_manager->create_playlist(p_name.data(), p_name_length, p_index);
      result.setResult(created_index);
    }
    
   void reorder(ApiParam<vector<int>> param,
                 ApiResult<bool> & result) {
      pfc::array_t<t_size> order;
      vector<int> permutations = param.value();
      order.set_size(permutations.size());

      for (t_size index = 0; index < permutations.size(); index++) {
        order[index] = permutations[index];
      }

      bool succesful = playlist_manager->reorder(order.get_ptr(), order.get_count());
      result.setResult(succesful);

    }

    void playlist_get_item_count(ApiParam<t_size> param, ApiResult<t_size> & result) {
      t_size p_playlist = param.value();
      t_size count = playlist_manager->playlist_get_item_count(p_playlist);
      result.setResult(count);

    }

    void playlist_get_focus_item(ApiParam<t_size> param, ApiResult<t_size> & result) {
      t_size p_playlist = param.value();
      t_size focus = playlist_manager->playlist_get_focus_item(p_playlist);
      result.setResult(focus);
    }

    void playlist_get_name(ApiParam<t_size> param, ApiResult<pfc::string8> & result) {
      t_size p_playlist = param.value();
      pfc::string8 temp;
      bool success = playlist_manager->playlist_get_name(p_playlist, temp);
          
      result.setResult(temp);
    }

    void playlist_reorder_items(ApiParam<tuple<t_size, vector<int>>> param,
                                ApiResult<bool> & result) {
      pfc::array_t<t_size> order;
      t_size p_playlist;
      vector<int> permutations;
      tie(p_playlist, permutations) = param.value();
      order.set_size(permutations.size());
      for (size_t index = 0; index < permutations.size(); index++) {
        order[index] = permutations[index];
      }

      bool success = playlist_manager->playlist_reorder_items(p_playlist, order.get_ptr(), order.get_size());
      result.setResult(success);
    }

    void playlist_set_selection(ApiParam<tuple<t_size, vector<t_size>, vector<bool>>> param, Event event) {
      t_size p_playlist;
      vector<t_size> p_affected;
      vector<bool> p_status;
      tie(p_playlist, p_affected, p_status) = param.value();

      if (p_status.size() != p_affected.size()) {
        console::formatter() << "The arrays passed to playlist_set_selection have different size.";
        event.set();
        return;
      }

      pfc::bit_array_var_impl p_affected_array;
      bit_array_bittable p_status_array(p_affected.size());      
      
      for (size_t index = 0; index < p_affected.size(); index++) {
        auto elem = p_affected[index];
        auto status = p_status[index];

        p_affected_array.set(elem);
        p_status_array.set(elem, status);
      }

      playlist_manager->playlist_set_selection(p_playlist, bit_array_true(), p_status_array);
      event.set();
    }

    void playlist_remove_items(ApiParam<tuple<t_size, vector<t_size>>> param, ApiResult<bool> & result) {
      t_size p_playlist;
      vector<t_size> array_mask;
      tie(p_playlist, array_mask) = param.value();

      bit_array_bittable table(array_mask.size());
      for (auto elem : array_mask) {
        table.set(elem, true);
      }

      bool successful = playlist_manager->playlist_remove_items(p_playlist, table);
      result.setResult(successful);
    }

    void playlist_replace_item(ApiParam<tuple<t_size, t_size, string>> param, ApiResult<bool> & result) {
      t_size p_playlist;
      t_size p_item;
      string path;
      tie(p_playlist, p_item, path) = param.value();

      metadb_handle_ptr handle;
      metadb_manager->handle_create(handle, make_playable_location(path.c_str(), 0));

      bool success = playlist_manager->playlist_replace_item(p_playlist, p_item, handle);

      result.setResult(success);
    }

    void playlist_set_focus_item(ApiParam<tuple<t_size, t_size>> param, Event event) {
      t_size p_playlist, p_item;
      tie(p_playlist, p_item) = param.value();

      playlist_manager->playlist_set_focus_item(p_playlist, p_item);

      event.set();
    }

    void playlist_insert_items(ApiParam<tuple<t_size, t_size, vector<string>>> param,
                               ApiResult<t_size> & result) {
      t_size p_playlist, p_base;
      vector<string> files;
      
      tie(p_playlist, p_base, files) = param.value();

      // Build the required handles.
      pfc::list_t<metadb_handle_ptr> handles;
      for (auto file : files) {
        metadb_handle_ptr handle;
        metadb_manager->handle_create(handle, make_playable_location(file.c_str(), 0));        
        handles.add_item(handle);
      }
      
      t_size res = playlist_manager->playlist_insert_items(
        p_playlist, p_base, handles, bit_array_true());

      result.setResult(res);
    }


    void playlist_ensure_visible(ApiParam<tuple<t_size, t_size>> param, Event event) {
      t_size p_playlist, p_item;
      tie(p_playlist, p_item) = param.value();

      playlist_manager->playlist_ensure_visible(p_playlist, p_item);

      event.set();
    }

    void playlist_rename(ApiParam<tuple<t_size, string>> param,
                         ApiResult<bool> &  result) {
      string name;
      t_size p_playlist;
      tie(p_playlist, name) = param.value();

      bool success = playlist_manager->playlist_rename(
        p_playlist, name.c_str(), name.length());

      result.setResult(success);

    }

    void playlist_undo_backup(ApiParam<t_size> param, Event event) {
      t_size p_playlist = param.value();
      playlist_manager->playlist_undo_backup(p_playlist);
      event.set();
    }

    void playlist_undo_restore(ApiParam<t_size> param, ApiResult<bool> & result) {
      t_size p_playlist = param.value();

      bool success = playlist_manager->playlist_undo_restore(p_playlist);

      result.setResult(success);
    }

    void playlist_redo_restore(ApiParam<t_size> param, ApiResult<bool> & result) {
      t_size p_playlist = param.value();

      bool success = playlist_manager->playlist_redo_restore(p_playlist);

      result.setResult(success);
    }

    void playlist_is_undo_available(ApiParam<t_size> param, ApiResult<bool> & result) {
      t_size p_playlist = param.value();

      bool success = playlist_manager->playlist_is_undo_available(p_playlist);

      result.setResult(success);
    }

    void playlist_is_redo_available(ApiParam<t_size> param, ApiResult<bool> & result) {
      t_size p_playlist = param.value();

      bool success = playlist_manager->playlist_is_redo_available(p_playlist);

      result.setResult(success);
    }

    void playlist_item_format_title(ApiParam<tuple<t_size, t_size, string>> param, ApiResult<pfc::string8>& result) {
      string format;
      pfc::string8 temp;
      t_size p_playlist, p_item;

      tie(p_playlist, p_item, format) = param.value();

      titleformat_object::ptr script;
      static_api_ptr_t<titleformat_compiler>()->compile_force(script, format.c_str());

      playlist_manager->playlist_item_format_title(
        p_playlist,
        p_item,
        NULL,
        temp,
        script.get_ptr(),
        NULL,
        playback_control::t_display_level::display_level_all
      );      
      result.setResult(temp);
    }

    void get_playing_item_location(ApiResult<tuple<bool, t_size, t_size>> & result) {
      t_size p_playlist = INFINITY;
      t_size p_index = INFINITY;

      bool success = playlist_manager->get_playing_item_location(&p_playlist, &p_index);

      result.setResult(make_tuple(success, p_playlist, p_index));
    }

    void playlist_sort_by_format(ApiParam<tuple<t_size, string, bool>> param, ApiResult<bool> & result) {
      t_size p_playlist;
      string format;
      bool p_sel_only;

      tie(p_playlist, format, p_sel_only) = param.value();
      
      bool success = playlist_manager->playlist_sort_by_format(
        p_playlist, format.c_str(), p_sel_only);

      result.setResult(success);

    }

    void playback_order_get_count(ApiResult<t_size> & result) {
      t_size order = playlist_manager->playback_order_get_count();

      result.setResult(order);
    }

    void playback_order_get_name(ApiParam<t_size> param, ApiResult<string> & result) {
      t_size p_index = param.value();

      const char* name = playlist_manager->playback_order_get_name(p_index);

      string temp(name);
      result.setResult(temp);
    }

    void playback_order_get_active(ApiResult<t_size> & result) {
      t_size index = playlist_manager->playback_order_get_active();
      result.setResult(index);
    }

    void playback_order_set_active(ApiParam<t_size> param, Event event) {
      t_size p_index = param.value();

      playlist_manager->playback_order_set_active(p_index);

      event.set();
    }

  };
}
