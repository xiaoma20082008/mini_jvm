//
// Created by machunxiao on 2020/4/7.
//

#ifndef MINI_JVM_FIELD_H
#define MINI_JVM_FIELD_H
#include "Attribute.h"
#include "Common.h"
#include <vector>
namespace jvm {
class Field {
public:
  Field(ClassReader *reader);
  Field(u2 access_flags_, u2 name_index_, u2 descriptor_index,
        Attributes *attributes);
  ~Field();

public:
  u2 access_flags;
  u2 name_index;
  u2 descriptor_index;
  Attributes *attributes;
};
} // namespace jvm
#endif // MINI_JVM_FIELD_H
