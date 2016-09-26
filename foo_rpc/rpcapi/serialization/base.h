#pragma once

#include <vector>
#include <string>

#include "../../percolate.h"

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
    Payload packed(T value) {};

    template<typename T>
    Payload packed(ApiResult<T> result) {};
  };
}
