#pragma once
// Generated on 2016-07-21 12:09:00
#pragma once
#include <string>
#include <map>
#include <tuple>

#include "../../stdafx.h"
#include "../../event.h"
#include "../../percolate.h"
#include "../../api/metadb_handle.h"
#include "../factory.h"
#include "optional.hpp"

using namespace std;
using namespace std::experimental;
using namespace serialization;


class RpcMetadbHandle {
private:  
  foobar::MetadbHandle api;
public:

  Payload format_title(vector<char> & buffer) {
    ApiParam<tuple<t_size, t_size, string> > param(serialization::serializer.unpack<tuple<t_size, t_size, string>>(buffer));
    ApiResult<optional<string> > result;
    fb2k::inMainThread([&] {
      api.format_title(param, result);
    });
    result.wait();
    return serialization::serializer.packed(result);
  }
};
