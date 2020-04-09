//
// Created by machunxiao on 2020/4/7.
//

#ifndef MINI_JVM_METHOD_H
#define MINI_JVM_METHOD_H
#include "Attribute.h"
#include "Common.h"
#include <vector>
namespace jvm {
class Method {
public:
  Method(ClassReader *reader);
  Method(u2 access_flags_, u2 name_index_, u2 descriptor_index,
         Attributes *attributes);
  ~Method();

public:
  u2 access_flags;
  u2 name_index;
  u2 descriptor_index;
  Attributes *attributes;
};
} // namespace jvm
#endif // MINI_JVM_METHOD_H
