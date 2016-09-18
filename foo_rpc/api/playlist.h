#pragma once
#include <map>
#include <vector>

#include "../stdafx.h"
#include "../percolate.h"
#include "track.h"
#include "callbacks.h"

using namespace std;


namespace foobar {
  class Playlist {
  private:
    static_api_ptr_t<playlist_manager> playlist_manager;
    static_api_ptr_t<playlist_manager_v4> playlist_manager_v4;
    static_api_ptr_t<metadb> metadb_manager;

    OptionalTrack get_item_as_track(t_size p_item, t_size p_playlist) {
      enum_items_callback_retrieve_item callback;
      playlist_manager->playlist_enum_items(
        p_playlist, callback, bit_array_one(p_item));

      return callback.get_item();
    }

    pfc::list_t<metadb_handle_ptr> metadb_handles_from_files(vector<string> files) {      
      pfc::list_t<metadb_handle_ptr> handles;
      for (auto file : files) {
        metadb_handle_ptr handle;
        metadb_manager->handle_create(handle, make_playable_location(file.c_str(), 0));
        handles.add_item(handle);
      }
      return handles;
    }
    
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

    void create_playlist(ApiParam <tuple<string, t_size, t_size>> param,
                         ApiResult<t_size> & result) {
      string p_name;
      t_size p_name_length, p_index;
      tie(p_name, p_name_length, p_index) = param.value();

      t_size created_index = playlist_manager->create_playlist(p_name.data(), p_name_length, p_index);
      result.setResult(created_index);
    }

    void create_playlist_ex(ApiParam<tuple<string, t_size, t_size, vector<string>>> param,
                            ApiResult<t_size> & result) {
      string p_name;
      t_size p_name_length, p_index;
      vector<string> files;

      tie(p_name, p_name_length, p_index, files) = param.value();

      auto handles = metadb_handles_from_files(files);

      abort_callback_dummy abort;
      stream_reader_dummy dummy_reader;

      metadb_handle_list contents;
      contents.add_items(handles);

      t_size index = playlist_manager_v4->create_playlist_ex(
        p_name.c_str(), p_name_length, p_index, contents, &dummy_reader, abort);
      result.setResult(index);

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

    void activeplaylist_reorder_items(ApiParam<vector<int>> param, ApiResult<bool> & result) {
      t_size playlist = playlist_manager->get_active_playlist();
      
      ApiParam<tuple<t_size, vector<int>>> passthrough(make_tuple(playlist, param.value()));

      playlist_reorder_items(passthrough, result);
    }

    void activeplaylist_set_selection(ApiParam<tuple<vector<t_size>, vector<bool>>> param, Event event) {
      vector<t_size> p_affected;
      vector<bool> p_status;
      t_size playlist = playlist_manager->get_active_playlist();

      tie(p_affected, p_status) = param.value();

      ApiParam<tuple<t_size, vector<t_size>, vector<bool>>> passthrough(
        make_tuple(playlist, p_affected, p_status));

      playlist_set_selection(passthrough, event);
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
    
    void activeplaylist_remove_items(ApiParam<vector<t_size>> param, ApiResult<bool> & result) {
      t_size playlist = playlist_manager->get_active_playlist();

      ApiParam<tuple<t_size, vector<t_size>>> passthrough(
        make_tuple(playlist, param.value()));

      playlist_remove_items(passthrough, result);
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

    void activeplaylist_replace_item(ApiParam<tuple<t_size, string>> param,
                                     ApiResult<bool> & result) {
      t_size playlist = playlist_manager->get_active_playlist();

      ApiParam<tuple<t_size, t_size, string>> passthrough(
        tuple_cat(make_tuple(playlist), param.value()));

      playlist_replace_item(passthrough, result);

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

    void activeplaylist_set_focus_item(ApiParam<t_size> param, Event event) {
      t_size playlist = playlist_manager->get_active_playlist();

      ApiParam<tuple<t_size, t_size>> passthrough(
        make_tuple(playlist, param.value()));
      playlist_set_focus_item(passthrough, event);
    }

    void activeplaylist_insert_items(ApiParam<tuple<t_size, vector<string>>> param,
                                     ApiResult<t_size> & result) {
      t_size playlist = playlist_manager->get_active_playlist();

      ApiParam<tuple<t_size, t_size, vector<string>>> passthrough(
        tuple_cat(make_tuple(playlist), param.value()));

      playlist_insert_items(passthrough, result);

    }

    void activeplaylist_add_items(ApiParam<vector<string>> param, ApiResult<t_size> & result) {
      t_size playlist = playlist_manager->get_active_playlist();

      ApiParam<tuple<t_size, vector<string>>> passthrough(
        make_tuple(playlist, param.value())
      );

      playlist_add_items(passthrough, result);
    }

    void playlist_add_items(ApiParam<tuple<t_size, vector<string>>> param,
                            ApiResult<t_size> & result) {
      t_size playlist;
      vector<string> paths;
      tie(playlist, paths) = param.value();;

      ApiParam<tuple<t_size, t_size, vector<string>>> passthrough(
        make_tuple(playlist, pfc_infinite, paths)
      );

      playlist_insert_items(passthrough, result);

    }

    void playlist_insert_items(ApiParam<tuple<t_size, t_size, vector<string>>> param,
                               ApiResult<t_size> & result) {
      t_size p_playlist, p_base;
      vector<string> files;
      
      tie(p_playlist, p_base, files) = param.value();
      
      auto handles = metadb_handles_from_files(files);
      
      t_size res = playlist_manager->playlist_insert_items(
        p_playlist, p_base, handles, bit_array_true());

      result.setResult(res);
    }

    void activeplaylist_ensure_visible(ApiParam<t_size> param, Event event) {
      t_size playlist = playlist_manager->get_active_playlist();

      ApiParam<tuple<t_size, t_size>> passthrough(
        make_tuple(playlist, param.value()));

      playlist_ensure_visible(passthrough, event);
    }

    void playlist_ensure_visible(ApiParam<tuple<t_size, t_size>> param, Event event) {
      t_size p_playlist, p_item;
      tie(p_playlist, p_item) = param.value();

      playlist_manager->playlist_ensure_visible(p_playlist, p_item);

      event.set();
    }

    void activeplaylist_rename(ApiParam<string> param, ApiResult<bool> & result) {
      t_size playlist = playlist_manager->get_active_playlist();

     ApiParam<tuple<t_size, string>> passthrough(
       make_tuple(playlist, param.value()));

      playlist_rename(passthrough, result);
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

    void activeplaylist_undo_backup(Event event) {
      t_size playlist = playlist_manager->get_active_playlist();
      
      ApiParam<t_size> param(playlist);

      playlist_undo_backup(param, event);
    }

    void playlist_undo_backup(ApiParam<t_size> param, Event event) {
      t_size p_playlist = param.value();
      playlist_manager->playlist_undo_backup(p_playlist);
      event.set();
    }

    void activeplaylist_undo_restore(ApiResult<bool> & result) {
      t_size playlist = playlist_manager->get_active_playlist();

      ApiParam<t_size> param(playlist);

      playlist_undo_restore(param, result);
    }

    void playlist_undo_restore(ApiParam<t_size> param, ApiResult<bool> & result) {
      t_size p_playlist = param.value();

      bool success = playlist_manager->playlist_undo_restore(p_playlist);

      result.setResult(success);
    }

    void activeplaylist_redo_restore(ApiResult<bool> & result) {
      t_size playlist = playlist_manager->get_active_playlist();

      ApiParam<t_size> param(playlist);

      playlist_redo_restore(param, result);
    }

    void playlist_redo_restore(ApiParam<t_size> param, ApiResult<bool> & result) {
      t_size p_playlist = param.value();

      bool success = playlist_manager->playlist_redo_restore(p_playlist);

      result.setResult(success);
    }

    void activeplaylist_is_undo_available(ApiResult<bool> & result) {
      t_size playlist = playlist_manager->get_active_playlist();

      ApiParam<t_size> param(playlist);

      playlist_is_undo_available(param, result);
    }

    void playlist_is_undo_available(ApiParam<t_size> param, ApiResult<bool> & result) {
      t_size p_playlist = param.value();

      bool success = playlist_manager->playlist_is_undo_available(p_playlist);

      result.setResult(success);
    }

    void activeplaylist_is_redo_available(ApiResult<bool> & result) {
      t_size playlist = playlist_manager->get_active_playlist();

      ApiParam<t_size> param(playlist);

      playlist_is_redo_available(param, result);
    }

    void playlist_is_redo_available(ApiParam<t_size> param, ApiResult<bool> & result) {
      t_size p_playlist = param.value();

      bool success = playlist_manager->playlist_is_redo_available(p_playlist);

      result.setResult(success);
    }

    void activeplaylist_item_format_title(ApiParam<tuple<t_size, string>> param, ApiResult<pfc::string8>& result) {
      t_size playlist = playlist_manager->get_active_playlist();

      ApiParam<tuple<t_size, t_size, string>> passthrough(
        tuple_cat(
          make_tuple(playlist),
          param.value()
        )
      );

      playlist_item_format_title(passthrough, result);

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

    void activeplaylist_sort_by_format(ApiParam<tuple<string, bool>> param, ApiResult<bool> & result) {
      t_size playlist = playlist_manager->get_active_playlist();

      ApiParam<tuple<t_size, string, bool>> passthrough(
        tuple_cat(make_tuple(playlist), param.value())
      );
      playlist_sort_by_format(passthrough, result);
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

    void queue_add_item_playlist(ApiParam<tuple<t_size, t_size>> param, Event event) {
      t_size p_playlist, p_item;
      tie(p_playlist, p_item) = param.value();

      playlist_manager->queue_add_item_playlist(p_playlist, p_item);

      event.set();

    }

    void queue_add_item(ApiParam<string> param, Event event) {
      metadb_handle_ptr handle;
      string file;

      file = param.value();
      
      metadb_manager->handle_create(handle, make_playable_location(file.c_str(), 0));
      
      playlist_manager->queue_add_item(handle);

      event.set();
    }

    void queue_get_count(ApiResult<t_size> & result) {
      t_size count = playlist_manager->queue_get_count();

      result.setResult(count);
    }

    void queue_get_contents(ApiResult<vector<t_playback_queue_item>>  & result) {
      vector<t_playback_queue_item> contents;

      pfc::list_t<t_playback_queue_item> handles;

      playlist_manager->queue_get_contents(handles);

      handles.for_each([&](auto handle) {
        contents.push_back(handle);
      });
      
      result.setResult(contents);
      
    }

    void queue_remove_mask(ApiParam<vector<t_size>> param, Event event) {
      vector<t_size> elems = param.value();
      bit_array_bittable mask(elems.size());

      for (auto elem : elems) {
        mask.set(elem, true);
      }

      playlist_manager->queue_remove_mask(mask);

      event.set();
    }

    void queue_flush(Event event) {
      playlist_manager->queue_flush();

      event.set();
    }

    void queue_is_active(ApiResult<bool> & result) {
      bool is_active = playlist_manager->queue_is_active();

      result.setResult(is_active);
    }

    void highlight_playing_item(ApiResult<bool> & result) {
      bool success = playlist_manager->highlight_playing_item();

      result.setResult(success);
    }

    void remove_playlist(ApiParam<t_size> param, ApiResult<bool> & result) {
      t_size p_playlist = param.value();

      bool success = playlist_manager->remove_playlist(p_playlist);

      result.setResult(success);
    }

    void remove_playlist_switch(ApiParam<t_size> param, ApiResult<bool> & result) {
      t_size p_playlist = param.value();

      bool success = playlist_manager->remove_playlist_switch(p_playlist);

      result.setResult(success);
    }

    void activeplaylist_is_item_selected(ApiParam<t_size> param, ApiResult<bool> & result) {
      t_size p_playlist = playlist_manager->get_active_playlist();

      ApiParam<tuple<t_size, t_size>> passthrough(
        make_tuple(p_playlist, param.value()));

      playlist_is_item_selected(passthrough, result);
    }

    void playlist_is_item_selected(ApiParam<tuple<t_size, t_size>> param, ApiResult<bool> & result) {
      t_size p_playlist;
      t_size p_item;
      tie(p_playlist, p_item) = param.value();

      bool success = playlist_manager->playlist_is_item_selected(p_playlist, p_item);

      result.setResult(success);
    }

    void activeplaylist_move_selection(ApiParam<int> param, ApiResult<bool> & result) {
      t_size playlist = playlist_manager->get_active_playlist();

      ApiParam<tuple<t_size, int>> passthrough(
        make_tuple(playlist, param.value())
      );

      playlist_move_selection(passthrough, result);
    }

    void playlist_move_selection(ApiParam<tuple<t_size, int>> param, ApiResult<bool> & result) {
      int p_delta;
      t_size p_playlist;
      tie(p_playlist, p_delta) = param.value();

      bool success = playlist_manager->playlist_move_selection(p_playlist, p_delta);

      result.setResult(success);
    }

    void activeplaylist_clear(Event event) {
      t_size playlist = playlist_manager->get_active_playlist();

      ApiParam<t_size> param(playlist);

      playlist_clear(param, event);
    }

    void playlist_clear(ApiParam<t_size> param, Event event) {
      t_size p_playlist = param.value();

      playlist_manager->playlist_clear(p_playlist);

      event.set();
    }

    void activeplaylist_clear_selection(Event event) {
      t_size playlist = playlist_manager->get_active_playlist();

      ApiParam<t_size> param(playlist);

      playlist_clear_selection(param, event);
    }

    void playlist_clear_selection(ApiParam<t_size> param, Event event) {
      t_size p_selection = param.value();

      playlist_manager->playlist_clear_selection(p_selection);

      event.set();

    }

    void activeplaylist_remove_selection(ApiParam<bool> param, Event event) {
      t_size playlist = playlist_manager->get_active_playlist();

      ApiParam<tuple<t_size, bool>> passthrough(
        make_tuple(playlist, param.value()));

      playlist_remove_selection(passthrough, event);

    }

    void playlist_remove_selection(ApiParam<tuple<t_size, bool>> param, Event event) {
      t_size p_playlist;
      bool p_crop;

      tie(p_playlist, p_crop) = param.value();

      playlist_manager->playlist_remove_selection(p_playlist, p_crop);

      event.set();
    }

    void active_playlist_get_name(ApiResult<pfc::string8> & result) {      
      pfc::string8 temp;
      bool success = playlist_manager->activeplaylist_get_name(temp);

      result.setResult(temp);
    }

    void activeplaylist_get_item_count(ApiResult<t_size> & result) {
      t_size count = playlist_manager->activeplaylist_get_item_count();

      result.setResult(count);
    }

    void activeplaylist_get_focus_item(ApiResult<t_size> & result) {
      t_size focus_item = playlist_manager->activeplaylist_get_focus_item();

      result.setResult(focus_item);
    }

    void create_playlist_autoname(ApiParam<t_size> param, ApiResult<t_size> & result) {
      t_size playlist = param.value();

      t_size playlist_id = playlist_manager->create_playlist_autoname(
        playlist
      );

      result.setResult(playlist_id);
    }

    void reset_playing_playlist(Event event) {
      playlist_manager->reset_playing_playlist();

      event.set();
    }

    void find_playlist(ApiParam<tuple<string, t_size>> param, ApiResult<t_size> & result) {
      string name;
      t_size length;
      tie(name, length) = param.value();

      t_size index = playlist_manager->find_playlist(name.c_str(), length);

      result.setResult(index);
    }

    void find_or_playlist(ApiParam<tuple<string, t_size>> param, ApiResult<t_size> & result) {
      string name;
      t_size length;
      tie(name, length) = param.value();

      t_size index = playlist_manager->find_or_create_playlist(name.c_str(), length);

      result.setResult(index);
    }

    void find_or_create_playlist_unlocked(ApiParam<tuple<string, t_size>> param,
                                          ApiResult<t_size> & result) {
      string name;
      t_size length;
      tie(name, length) = param.value();

      t_size index = playlist_manager->find_or_create_playlist_unlocked(
        name.c_str(), length);

      result.setResult(index);
    }

    void active_playlist_fix(Event event) {
      playlist_manager->active_playlist_fix();

      event.set();
    }

    void playlist_activate_delta(ApiParam<int> param, Event event) {
      int t_delta = param.value();

      playlist_manager->playlist_activate_delta(t_delta);

      event.set();

    }

    void playlist_activate_next(Event event) {
      playlist_manager->playlist_activate_next();

      event.set();
    }

    void playlist_activate_previous(Event event) {
      playlist_manager->playlist_activate_previous();

      event.set();
    }

    void playlist_get_selection_count(ApiParam<tuple<t_size, t_size>> param, ApiResult<t_size> & result) {
      t_size playlist;
      t_size p_max;
      tie(playlist, p_max) = param.value();

      t_size count = playlist_manager->playlist_get_selection_count(playlist, p_max);

      result.setResult(count);

    }

    void activeplaylist_get_selection_count(ApiParam<t_size> param, ApiResult<t_size> & result) {
      t_size playlist = playlist_manager->get_active_playlist();

      ApiParam<tuple<t_size, t_size>> passthrough(
        make_tuple(playlist, param.value()));

      playlist_get_selection_count(passthrough, result);
    }

    void playlist_set_selection_single(ApiParam<tuple<t_size, t_size, bool>> param, Event event) {
      t_size playlist;
      t_size item;
      bool state;
      tie(playlist, item, state) = param.value();

      playlist_manager->playlist_set_selection_single(playlist, item, state);
      
      event.set();
    }

    void activeplaylist_set_selection_single(ApiParam<tuple<t_size, bool>> param, Event event) {
      t_size playlist = playlist_manager->get_active_playlist();

      ApiParam<tuple<t_size, t_size, bool>> passthrough(
        tuple_cat(make_tuple(playlist), param.value())
      );

      playlist_set_selection_single(passthrough, event);
    }

    void activeplaylist_get_all_items(ApiResult<vector<Track>> & result) {
      t_size playlist = playlist_manager->get_active_playlist();

      ApiParam<t_size> param(playlist);

      playlist_get_all_items(param, result);
    }

    void playlist_get_all_items(ApiParam<t_size> param,
                                ApiResult<vector<Track>> & result) {

      t_size playlist = param.value();

      enum_items_callback_all enum_callback;
      playlist_manager->playlist_enum_items(playlist, enum_callback, bit_array_true());

      result.setResult(enum_callback.m_out);
    }

    void activeplaylist_get_selected_items(ApiResult<vector<Track>> & result) {
      t_size playlist = playlist_manager->get_active_playlist();

      ApiParam<t_size> param(playlist);

      playlist_get_selected_items(param, result);
    }
    
    void playlist_get_selected_items(ApiParam<t_size> param, ApiResult<vector<Track>> & result) {
      t_size playlist = param.value();

      enum_items_callback_retrieve_selected_items enum_callback;

      playlist_manager->playlist_enum_items(playlist, enum_callback, bit_array_true());

      result.setResult(enum_callback.m_out);
    }

    void activeplaylist_get_item_handle(ApiParam<t_size> param, ApiResult<OptionalTrack> & result) {
      t_size playlist = playlist_manager->get_active_playlist();

      ApiParam<tuple<t_size, t_size>> passthrough(
        make_tuple(playlist, param.value()));

      playlist_get_item_handle(passthrough, result);

    }

    void playlist_get_item_handle(ApiParam<tuple<t_size, t_size>> param, ApiResult<OptionalTrack> & result)
    {
      t_size p_playlist, p_item;
      tie(p_playlist, p_item) = param.value();
      result.setResult(get_item_as_track(p_item, p_playlist));
    }

    void playlist_get_focus_item_handle(ApiParam<t_size> param, ApiResult<OptionalTrack> & result) {
      t_size playlist = param.value();
      t_size item = playlist_manager->playlist_get_focus_item(playlist);
      auto track = get_item_as_track(item, playlist);

      result.setResult(track);
    }

    void activeplaylist_get_focus_item_handle(ApiResult<OptionalTrack> & result) {
      t_size playlist = playlist_manager->get_active_playlist();
      ApiParam<t_size> param(playlist);

      playlist_get_focus_item_handle(param, result);
    }

    void activeplaylist_get_items(ApiParam<vector<t_size>> param, ApiResult<vector<Track>> & result) {
      t_size playlist = playlist_manager->get_active_playlist();

      ApiParam<tuple<t_size, vector<t_size>>> passthrough(
        make_tuple(playlist, param.value())
      );

      playlist_get_items(passthrough, result);
    }

    void playlist_get_items(ApiParam<tuple<t_size, vector<t_size>>> param, ApiResult<vector<Track>> & result) {
      t_size playlist;
      vector<t_size> masks;
      vector<Track> tracks;
      tie(playlist, masks) = param.value();

      bit_array_bittable table(masks.size());
      for (auto elem : masks) {
        table.set(elem, true);
      }

      enum_items_callback_all callback;
      playlist_manager->playlist_enum_items(playlist, callback, table);

      result.setResult(callback.m_out);
    }

    void playlist_insert_items_filter(ApiParam<tuple<t_size, t_size, bool, vector<string>>> param, ApiResult<bool> & result) {
      vector<string> files;
      t_size playlist;
      t_size p_base;
      bool p_select;

      tie(playlist, p_base, p_select, files) = param.value();

      metadb_handle_list temp;
      static_api_ptr_t<playlist_incoming_item_filter> api;

      auto handles = metadb_handles_from_files(files);

      if (!api->filter_items(handles, temp)) {
        result.setResult(false);
        return;
      }

      t_size result_value = playlist_manager->playlist_insert_items(
        playlist, p_base, temp, bit_array_val(p_select)
      );

      result.setResult(result_value != pfc_infinite);            
  }

  };
}
