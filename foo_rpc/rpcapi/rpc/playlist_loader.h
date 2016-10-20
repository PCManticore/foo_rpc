#pragma once
#include <string>
#include <map>
#include <tuple>

#include "../../stdafx.h"
#include "../../event.h"
#include "../../percolate.h"
#include "../../api/playlist_loader.h"
#include "../factory.h"

using namespace std;
using namespace serialization;


class RpcPlaylistLoader {
private:
  foobar::PlaylistLoader api;
public:

  Payload load_playlist(vector<char> & buffer) {
    ApiResult<bool> result;
    ApiParam<string> param(serialization::serializer.unpack<string>(buffer));
    fb2k::inMainThread([&] {
      api.load_playlist(param, result);
    });
    result.wait();
    return serialization::serializer.packed(make_tuple(0, result));
  }

  Payload save_playlist(vector<char> & buffer) {
    ApiResult<bool> result;
    ApiParam<tuple<string, vector<string>>> param(serialization::serializer.unpack<tuple<string, vector<string>>>(buffer));
    fb2k::inMainThread([&] {
      api.save_playlist(param, result);
    });
    result.wait();
    return serialization::serializer.packed(make_tuple(0, result));
  }

};

