//
// Created by machunxiao on 2020/4/4.
//

#ifndef MINI_JVM_CLASSFILE_H
#define MINI_JVM_CLASSFILE_H

#include "ConstantPool.h"

namespace jvm {

struct ClassFile {
  u4 magic;
  u2 minor_version;
  u2 major_version;
  u2 constant_pool_count;
  CpInfo *constant_pool;
  u2 access_flags;
  u2 this_class;
  u2 super_class;
  u2 interfaces_count;
  u2 *interfaces;
  u2 fields_count;
  FieldInfo *fields;
  u2 methods_count;
  MethodInfo *methods;
  u2 attributes_count;
  AttributeInfo *attributes;
};

} // namespace jvm
#endif // MINI_JVM_CLASSFILE_H
