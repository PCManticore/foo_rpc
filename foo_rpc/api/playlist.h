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

    // TODO: virtual bool playlist_replace_item(t_size p_playlist,t_size p_item,const metadb_handle_ptr & p_new_item) = 0;

    //void playlist_set_focus_item(ApiParam<tuple<t_size, t_size>> param, Event event) {
     // t_size 
    //}
    //! Sets index of focus item on specified playlist; use infinite to set no focus item.
    //virtual void playlist_set_focus_item(t_size p_playlist, t_size p_item) = 0;

  };
}
