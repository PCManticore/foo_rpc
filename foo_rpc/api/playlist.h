#pragma once
#include <map>
#include <vector>

#include "../stdafx.h"
#include "../percolate.h"
#include "common/track.h"
#include "common/callbacks.h"
#include "common/helpers.h"
#include "optional.hpp"

using namespace std;
using namespace std::experimental;


namespace foobar {
  class Playlist {
  private:
    static_api_ptr_t<playlist_manager> playlist_manager;
    static_api_ptr_t<playlist_manager_v4> playlist_manager_v4;
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
      pfc::bit_array_var_impl table;
      for (auto elem : elements) {           
        table.set(elem);
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

      pfc::bit_array_var_impl table;
      for (auto elem : array_mask) {
        table.set(elem);
      }

      bool successful = playlist_manager->playlist_remove_items(p_playlist, table);
      result.setResult(successful);
    }

    void activeplaylist_replace_item(ApiParam<tuple<t_size, t_size>> param,
                                     ApiResult<bool> & result) {
      t_size playlist = playlist_manager->get_active_playlist();

      ApiParam<tuple<t_size, t_size, t_size>> passthrough(
        tuple_cat(make_tuple(playlist), param.value()));

      playlist_replace_item(passthrough, result);

    }

    void playlist_replace_item(ApiParam<tuple<t_size, t_size, t_size>> param, ApiResult<bool> & result) {
      t_size p_playlist;
      t_size origin;
      t_size replacee;
      string path;
      tie(p_playlist, origin, replacee) = param.value();

      metadb_handle_ptr handle = playlist_manager->playlist_get_item_handle(p_playlist, replacee);

      bool success = playlist_manager->playlist_replace_item(p_playlist, origin, handle);

      result.setResult(success);
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

    void activeplaylist_rename(ApiParam<tuple<string, t_size>> param, ApiResult<bool> & result) {
      t_size length;
      t_size playlist = playlist_manager->get_active_playlist();

     ApiParam<tuple<t_size, string, t_size>> passthrough(
       tuple_cat(
         make_tuple(playlist),
         param.value()
       ));

      playlist_rename(passthrough, result);
    }

    void playlist_rename(ApiParam<tuple<t_size, string, t_size>> param,
                         ApiResult<bool> &  result) {
      string name;
      t_size p_playlist;
      t_size length;
      tie(p_playlist, name, length) = param.value();

      bool success = playlist_manager->playlist_rename(
        p_playlist, name.c_str(), length);

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
      t_size p_playlist = pfc::infinite32;
      t_size p_index = pfc::infinite32;

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

    void queue_get_count(ApiResult<t_size> & result) {
      t_size count = playlist_manager->queue_get_count();

      result.setResult(count);
    }

    void queue_get_contents(ApiResult<vector<t_playback_queue_item>>  & result) {
      vector<t_playback_queue_item> contents;

      pfc::list_t<t_playback_queue_item> handles;

      playlist_manager->queue_get_contents(handles);

      handles.for_each([&](t_playback_queue_item handle) {
        contents.push_back(handle);
      });
      
      result.setResult(contents);
      
    }

    void queue_remove_mask(ApiParam<vector<t_size>> param, Event event) {
      vector<t_size> elems = param.value();
      pfc::bit_array_var_impl mask;

      for (auto elem : elems) {
        mask.set(elem);
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

    void activeplaylist_get_name(ApiResult<pfc::string8> & result) {      
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

    void find_or_create_playlist(ApiParam<tuple<string, t_size>> param, ApiResult<t_size> & result) {
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

    void activeplaylist_get_item_handle(ApiParam<t_size> param,
                                        ApiResult<optional<Track>> & result) {
      t_size playlist = playlist_manager->get_active_playlist();

      ApiParam<tuple<t_size, t_size>> passthrough(
        make_tuple(playlist, param.value()));

      playlist_get_item_handle(passthrough, result);

    }

    void playlist_get_item_handle(ApiParam<tuple<t_size, t_size>> param,
                                  ApiResult<optional<Track>> & result)
    {
      t_size p_playlist, p_item;
      tie(p_playlist, p_item) = param.value();
      result.setResult(get_item_as_track(p_item, p_playlist));
    }

    void playlist_get_focus_item_handle(ApiParam<t_size> param, ApiResult<optional<Track>> & result) {
      t_size playlist = param.value();
      t_size item = playlist_manager->playlist_get_focus_item(playlist);
      auto track = get_item_as_track(item, playlist);

      result.setResult(track);
    }

    void activeplaylist_get_focus_item_handle(ApiResult<optional<Track>> & result) {
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
