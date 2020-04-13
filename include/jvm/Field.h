//
// Created by machunxiao on 2020/4/7.
//

#ifndef MINI_JVM_FIELD_H
#define MINI_JVM_FIELD_H
#include "Attribute.h"
#include "Common.h"
#include <vector>
namespace jvm {
class ConstantPool;
class Field {
public:
  Field(ClassReader *reader);
  Field(u2 access_flags_, u2 name_index_, u2 descriptor_index,
        Attributes *attributes);
  ~Field();

  std::string GetName(ConstantPool *pool);

public:
  u2 access_flags = 0;
  u2 name_index = 0;
  u2 descriptor_index = 0;
  Attributes *attributes = nullptr;
};
} // namespace jvm
#endif // MINI_JVM_FIELD_H
