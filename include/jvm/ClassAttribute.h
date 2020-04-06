//
// Created by machunxiao on 2020/4/6.
//

#ifndef MINI_JVM_CLASSATTRIBUTE_H
#define MINI_JVM_CLASSATTRIBUTE_H
#include "Common.h"

namespace jvm {

// region attribute

struct Attribute {
  u2 attribute_name_index;
  u4 attribute_length;
};

// endregion attribute

} // namespace jvm
#endif // MINI_JVM_CLASSATTRIBUTE_H
