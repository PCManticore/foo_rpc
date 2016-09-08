#pragma once

namespace serialization {  
  class Payload {
  public:
    string data;
    int size;

    Payload(string value, int value_size) {
      data = value;
      size = value_size;
    }
  };
  
  class Serializer {
  public:    

    template<typename T>
    T unpack(vector<char> buf) {};

    template<typename T>
    Payload packed_bool(bool value) {};

    template<typename T>
    Payload packed_result(ApiResult<T> result) {};
  };
}
