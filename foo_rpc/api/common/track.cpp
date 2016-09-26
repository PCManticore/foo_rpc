#include <string>

#include "track.h"

Track Track::newTrack(t_size index, const metadb_handle_ptr & handle, bool selected) {
  return Track(
    index,
    handle->get_subsong_index(),
    selected,
    std::string(handle->get_path()),
    handle
  );
}

Track::Track(t_size index_value,
             t_uint32 subsong_index_value,
             bool selected_value,
             std::string path_value,
             const metadb_handle_ptr & handle_value) :
             index(index_value),
             subsong_index(subsong_index_value),
             selected(selected_value),
             path(path_value),
             handle(handle_value) {}