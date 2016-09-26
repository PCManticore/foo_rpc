#pragma once

#include <string>

#include "../../stdafx.h"
#include "../../Optional/optional.hpp"

// A track contains the path and the index of a particular
// playlist item. This is used as a transport object between
// this component and the client.
class Track {
public:
  t_size index;  
  t_uint32 subsong_index;
  bool selected;
  std::string path;
  metadb_handle_ptr handle;

  static Track newTrack(t_size index, const metadb_handle_ptr & handle, bool selected);

  Track(t_size index_value,
    t_uint32 subsong_index_value,
    bool selected_value,
    std::string path_value,
    const metadb_handle_ptr & handle_value);

};
