#pragma once

#include <tuple>
#include <vector>
#include <string>

#include "../../percolate.h"

#define REQUEST_SUCCEEDED 0
#define REQUEST_FAILED 1


namespace serialization {  
  class Payload {
  public:
    std::string data;
    int size;

    Payload(std::string value, int value_size) {
      data = value;
      size = value_size;
    }
  };
  
  class Serializer {
  public:    

    template<typename T>
    T unpack(std::vector<char> buf) {};

    template<typename T>
    Payload packed(tuple<int, T> value) {};
    
    template<typename T>
    Payload packed(tuple<int, ApiResult<T>> result) {};

  };
}
