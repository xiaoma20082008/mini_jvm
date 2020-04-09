//
// Created by machunxiao on 2020/4/4.
//

#ifndef MINI_JVM_CLASSFILE_H
#define MINI_JVM_CLASSFILE_H

#include "Attribute.h"
#include "ConstantPool.h"
#include "Field.h"
#include "Method.h"

namespace jvm {

class ClassFile {
public:
  ClassFile(const char *file_);
  ClassFile(u4 magic_, u2 minor_version_, u2 major_version_,
            ConstantPool *constant_pool_, u2 access_flags_, u2 this_class_,
            u2 super_class_, u2 interfaces_count_, u2 *interfaces_,
            u2 fields_count_, Field **fields_, u2 methods_count_,
            Method **methods_, Attributes *attributes);
  ~ClassFile();

public:
  u4 magic;
  u2 minor_version;
  u2 major_version;
  ConstantPool *constant_pool;
  u2 access_flags;
  u2 this_class;
  u2 super_class;
  u2 interfaces_count;
  u2 *interfaces;
  u2 fields_count;
  Field **fields;
  u2 methods_count;
  Method **methods;
  Attributes *attributes;

private:
  ClassReader *reader;
  const char *file;
};

} // namespace jvm
#endif // MINI_JVM_CLASSFILE_H
