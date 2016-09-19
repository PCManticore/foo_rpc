#pragma once
#pragma once
#include <tuple>
#include <filesystem>

#include "../stdafx.h"
#include "../percolate.h"


class playlist_loader_cb : public playlist_loader_callback
{
private:  
  static_api_ptr_t<metadb> metadb_manager;  
  
public:
  
  pfc::list_t<metadb_handle_ptr> handles;

  playlist_loader_cb() {}

  int service_release() { return 0; }
  int service_add_ref() { return 1; }
  
  void on_entry(const metadb_handle_ptr & p_item, t_entry_type p_type, const t_filestats & p_stats, bool p_fresh) {
    handles.add_item(p_item);
  }
  
  void on_progress(const char * p_path) {}

  bool want_info(const metadb_handle_ptr & p_item, t_entry_type p_type, const t_filestats & p_stats, bool p_fresh) {
    return false;
  }

  void on_entry_info(const metadb_handle_ptr & p_item, t_entry_type p_type, const t_filestats & p_stats, const file_info & p_info, bool p_fresh) {
    
  }

  void handle_create(metadb_handle_ptr & p_out, const playable_location & p_location) {
    metadb_manager->handle_create(p_out, p_location);
  }

  bool is_path_wanted(const char * path, t_entry_type type) {
    return true;
  }

  bool want_browse_info(const metadb_handle_ptr & p_item, t_entry_type p_type, t_filetimestamp ts) {
    return false;
  }
  void on_browse_info(const metadb_handle_ptr & p_item, t_entry_type p_type, const file_info & info, t_filetimestamp ts) {

  }

};

namespace foobar {
  class PlaylistLoader {
  private:    
    static_api_ptr_t<metadb> metadb_manager;
    static_api_ptr_t<playlist_manager_v4> playlist_manager;
    
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

    void save_playlist(ApiParam<tuple<string, vector<string>>> param, ApiResult<bool> & result) {
      string path;
      vector<string> files;
      tie(path, files) = param.value();
      auto handles = metadb_handles_from_files(files);

      abort_callback_dummy abort;

      metadb_handle_list contents;
      contents.add_items(handles);
      playlist_loader::g_save_playlist(path.c_str(), contents, abort);
      result.setResult(true);
    }

    void load_playlist(ApiParam<string> param, ApiResult<bool> & result) {
      
      string path = param.value();

      abort_callback_dummy abort;      
      playlist_loader_cb cb;
      stream_reader_dummy dummy_reader;
      bool success;
      pfc::string filename = pfc::io::path::getFileNameWithoutExtension(path.c_str());
 
      try {    
        playlist_loader::g_try_load_playlist(NULL, path.c_str(), &cb, abort);  
        playlist_manager->create_playlist_ex(
          filename.c_str(),
          filename.length(),
          pfc_infinite,
          cb.handles,
          &dummy_reader,
          abort);
        success = true;
      }
      catch (exception_io & e) {
        console::formatter() << "Error happened when loading playlist: " << e;
        success = false;
      }
      catch (exception_aborted & e) {
        console::formatter() << "Loading playlist aborted: " << e;
        success = false;
      }
      result.setResult(success);
      
    }

  };
}
