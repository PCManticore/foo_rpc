#pragma once
#include "serialization/base.h"
#include "serialization/msgpack.h"

using namespace msgpack::adaptor;

namespace serialization {   
  serialization::Msgpack serializer = serialization::Msgpack();  
};