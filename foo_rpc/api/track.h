#pragma once

#include <string>

#include "../stdafx.h"

// A track contains the path and the index of a particular
// playlist item. This is used as a transport object between
// this component and the client.
class Track {
public:
  t_size index;  
  t_uint32 subsong_index;
  bool selected;
  std::string path;

  static Track newTrack(t_size index, const metadb_handle_ptr & location, bool selected) {    
    return Track(
      index,
      location->get_subsong_index(),
      selected,
      string(location->get_path())
    );
  }

  Track(t_size index_value,        
        t_uint32 subsong_index_value,
        bool selected_value,
        std::string path_value) :
    index(index_value),
    subsong_index(subsong_index_value),
    selected(selected_value),
    path(path_value) {}

};