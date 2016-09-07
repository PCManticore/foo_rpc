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
    static T unpack(vector<char> buf) {};

    template<typename T>
    static Payload packed_result(ApiResult<T> result) {};
  };
}
