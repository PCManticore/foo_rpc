#pragma once
#include <tuple>

#include "../stdafx.h"
#include "../percolate.h"

using namespace std;

namespace foobar {
  class CoreVersion {
  private:
    static_api_ptr_t<core_version_info> core_version_1;
    static_api_ptr_t<core_version_info_v2> core_version_2;

  public:
    void get_version_string(ApiResult<string> & result) {      
      const char * version_string = core_version_1->get_version_string();
      result.setResult(string(version_string));
    }

    void get_name(ApiResult<string> & result) {
      const char * name = core_version_2->get_name();
      result.setResult(string(name));
    }

    void get_version(ApiResult<tuple<t_uint32, t_uint32, t_uint32, t_uint32>> & result) {
      t_core_version_data data = core_version_2->get_version();
      result.setResult(make_tuple(data.m_major, data.m_minor1, data.m_minor2, data.m_minor3));
    }

    void get_version_as_text(ApiResult<string> & result) {
      const char * version = core_version_2->get_version_as_text();
      result.setResult(string(version));
    }

    void test_version(ApiParam<tuple<t_uint32, t_uint32, t_uint32, t_uint32>> param,
                      ApiResult<bool> & result) {
      t_uint32 major, minor1, minor2, minor3;
      tie(major, minor1, minor2, minor3) = param.value();
      bool versionCheck = core_version_2->test_version(major, minor1, minor2, minor3);
      result.setResult(versionCheck);

    };
  };
}
