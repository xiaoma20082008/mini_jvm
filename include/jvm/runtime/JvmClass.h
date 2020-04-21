//
// Created by machunxiao on 2020/4/17.
//

#ifndef MINI_JVM_JVMCLASS_H
#define MINI_JVM_JVMCLASS_H
#include "Common.h"
#include "classfile/ClassFile.h"
#include "classfile/ClassReader.h"
#include <unordered_map>

namespace jvm {

class JvmClass {
public:
  explicit JvmClass(const std::string& filepath) : reader(filepath) { Parse(); }
  ~JvmClass() { delete raw; };

  bool HasMethod(const std::string& name, const std::string& desc) const;

  Method* GetMethod(const std::string& name, const std::string& desc) const;

  Field* GetField(const std::string& name) const;

  bool Verify() { return true; }

  void Prepare() {}

private:
  void Parse();

private:
  ClassFile* raw{};
  ClassReader reader;
};
} // namespace jvm
#endif // MINI_JVM_JVMCLASS_H
