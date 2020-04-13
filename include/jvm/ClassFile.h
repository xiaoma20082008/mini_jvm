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
class AccessFlags;
class ClassFile {
public:
  ClassFile(const char *file_);
  ClassFile(u4 magic_, u2 minor_version_, u2 major_version_,
            ConstantPool *constant_pool_, u2 access_flags_, u2 this_class_,
            u2 super_class_, u2 interfaces_count_, u2 *interfaces_,
            u2 fields_count_, Field **fields_, u2 methods_count_,
            Method **methods_, Attributes *attributes);
  ~ClassFile();
  std::string ToString();
  bool IsClass();
  bool IsInterface();


public:
  u4 magic = 0;
  u2 minor_version = 0;
  u2 major_version = 0;
  ConstantPool *constant_pool = nullptr;
  AccessFlags *access_flags = nullptr;
  u2 this_class = 0;
  u2 super_class = 0;
  u2 interfaces_count = 0;
  u2 *interfaces = nullptr;
  u2 fields_count = 0;
  std::vector<Field *> fields;
  u2 methods_count = 0;
  std::vector<Method *> methods;
  Attributes *attributes = nullptr;

private:
  ClassReader *reader = nullptr;
  const char *file = nullptr;
};

} // namespace jvm
#endif // MINI_JVM_CLASSFILE_H
