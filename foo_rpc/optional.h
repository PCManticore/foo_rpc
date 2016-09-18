#pragma once

#include <vector>

using namespace std;

// A really simple Optional class, used when we have potential
// empty values in the RPC layer (not having a currently playing
// track, for example).
// This should probably be killed when we will have access
// to std::experimental::optional.

template <typename T>
struct Optional {
private:
  vector<T> vec;
  bool _empty = true;

public:

  Optional(T obj) {
    vec.push_back(obj);
    _empty = false;
  }

  static Optional<nullptr> empty() {
    Optional<nullptr>
  }

  bool is_empty() const { return _empty; }

  T get() {
    return vec.front();
  }
  
};
