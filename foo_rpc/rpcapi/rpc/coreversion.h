// Generated on 2016-14-08 16:09:11
#pragma once
#include <string>
#include <map>
#include <tuple>

#include "stdafx.h"
#include "event.h"
#include "percolate.h"
#include "api/coreversion.h"
#include "../factory.h"

using namespace std;
using namespace serialization;


class RpcCoreVersion {
  private:
    foobar::CoreVersion api;
  public:

    Payload get_version_string(vector<char> & buffer) {
      ApiResult<string> result;
      fb2k::inMainThread([&] {
        api.get_version_string(result);
      });
      result.wait();            
      return serialization::serializer.packed_result(result);
    }

    Payload get_name(vector<char> & buffer) {
      ApiResult<string> result;
      fb2k::inMainThread([&] {
        api.get_name(result);
      });
      result.wait();            
      return serialization::serializer.packed_result(result);
    }

    Payload get_version(vector<char> & buffer) {
      ApiResult<tuple<t_uint32, t_uint32, t_uint32, t_uint32> > result;
      fb2k::inMainThread([&] {
        api.get_version(result);
      });
      result.wait();            
      return serialization::serializer.packed_result(result);
    }

    Payload get_version_as_text(vector<char> & buffer) {
      ApiResult<string> result;
      fb2k::inMainThread([&] {
        api.get_version_as_text(result);
      });
      result.wait();            
      return serialization::serializer.packed_result(result);
    }

    Payload test_version(vector<char> & buffer) {
      ApiParam<tuple<t_uint32, t_uint32, t_uint32, t_uint32> > param(serialization::serializer.unpack<tuple<t_uint32, t_uint32, t_uint32, t_uint32>>(buffer));
      ApiResult<bool> result;
      fb2k::inMainThread([&] {
        api.test_version(param, result);
      });
      result.wait();
      return serialization::serializer.packed_result(result);
    }
};
