//
// Created by machunxiao on 2020/4/21.
//

#ifndef MINI_JVM_CLASSREADER_H
#define MINI_JVM_CLASSREADER_H
#include "Common.h"
#include "Exception.h"
#include "classfile/ClassFile.h"
#include "unordered_map"
#include "utils/FileIo.h"
namespace jvm {

class ClassReader {
public:
  explicit ClassReader(const std::string& filepath) : reader(filepath) {
    InitMap();
  }
  ~ClassReader() = default;
  ClassFile* Parse();

private:
  bool ParseConstantPool(ClassFile* raw, u2 constantPool_count_);
  bool ParseInterfaces(ClassFile* raw, u2 interfaces_count_);
  bool ParseFields(ClassFile* raw, u2 fields_count_);
  bool ParseMethods(ClassFile* raw, u2 methods_count_);
  bool ParseAttributes(ClassFile* raw, Attribute**(&attr),
                       u2 attributes_count_);
  ElementValue* ParseElementValue(ClassFile* raw);
  void InitMap();

private:
  io::FileReader reader;
  std::unordered_map<std::string, u2> attr_table;
};

class ClassWriter {};

} // namespace jvm
#endif // MINI_JVM_CLASSREADER_H
