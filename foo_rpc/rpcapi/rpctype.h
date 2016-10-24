#pragma once
#pragma once

#include <vector>
#include <string>
#include <sstream>

#include "../tinyformat/tinyformat.h"
#include "../stdafx.h"


template<typename T>
class repr_type {
public:
  static const std::string value() {
    return "...";
  }
};

namespace typeinfo
{

  std::string join_elems(const std::vector<std::string>& elems, std::string delimiter) {
    std::ostringstream stream;
    for (const auto & elem : elems) {
      if (&elem != &elems[0]) {
        stream << delimiter;
      }
      stream << elem;
    }
    return stream.str();
  }

  template<int... Is>
  struct seq { };

  template<int N, int... Is>
  struct gen_seq : gen_seq<N - 1, N - 1, Is...> { };

  template<int... Is>
  struct gen_seq<0, Is...> : seq<Is...> { };

  template<typename... T, int... Is>
  std::vector<std::string> for_each(seq<Is...>) {
    return{ (repr_type<std::tuple_element<Is, std::tuple<T...>>::type>::value())... };
  };

  template<typename... Ts>
  std::vector<std::string> for_each_in_tuple()
  {
    return for_each<Ts...>(gen_seq<sizeof...(Ts)>());
  };

}


template<>
class repr_type<int> {
public:
  static const std::string value() {
    return "int";
  }
};

template<>
class repr_type<bool> {
public:
  static const std::string value() {
    return "bool";
  }
};

template<>
class repr_type<float> {
public:
  static const std::string value() {
    return "float";
  }
};

template<>
class repr_type<double> {
public:
  static const std::string value() {
    return "double";
  }
};

template<>
class repr_type<std::string> {
public:
  static const std::string value() {
    return "string";
  }
};

template<>
class repr_type<t_size> {
public:
  static const std::string value() {
    return "size_t";
  }
};

template<typename... T>
class repr_type<std::tuple<T...>> {
public:
  static const std::string value() {
    std::vector<std::string> elems = typeinfo::for_each_in_tuple<T...>();
    std::string joined = typeinfo::join_elems(elems, ",");
    return tfm::format("tuple<%s>", joined);
  }
};


template<typename T>
class repr_type<std::vector<T>> {
public:
  static const std::string value() {
    auto inner_type = repr_type<vector<T>::value_type>::value();
    return tfm::format("vector<%s>", inner_type);
  }
};

template<>
class repr_type<play_control::t_track_command> {
public:
  static const std::string value() {
    return "enum t_track_command {track_command_default, "
      "track_command_play, track_command_next, "
      "track_command_prev, track_command_settrack, "
      "track_command_rand}";
  }
};

