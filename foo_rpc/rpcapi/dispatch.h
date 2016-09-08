#pragma once
#include <map>
#include <string>

#include "rpc_playback_control.h"
#include "factory.h"

using namespace std;
using namespace serialization;

namespace foobar {
  typedef map<string, std::function<Payload(vector<char> &)>> dispatch_map;

  class MethodDispatcher {
  private:    
    RpcPlaybackControl rpc_playback_control;    
    dispatch_map registry;
    
  public:

    MethodDispatcher() {            
      registry["PlaybackControl.playback_format_title_complete"] = [&](vector<char> & param) {
        return rpc_playback_control.playback_format_title_complete(param);
      };
      registry["PlaybackControl.play_or_pause"] = [&](vector<char> & param) {
        return rpc_playback_control.play_or_pause(param);
      };
      registry["PlaybackControl.set_volume"] = [&](vector<char> & param) {
        return rpc_playback_control.set_volume(param);
      };
    }

    Payload dispatch(vector<char> received) {
      string method_name;
      vector<char> buf;

      auto dst = serializer.unpack<tuple<string, vector<char>>>(received);
      tie(method_name, buf) = dst;

      dispatch_map::const_iterator iter = registry.find(method_name);
      if (iter == registry.end())
      {
        throw RPCException("Cannot find the given method.");
      }
      return iter->second(buf);
    }

  };

}
