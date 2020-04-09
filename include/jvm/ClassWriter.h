//
// Created by machunxiao on 2020/4/6.
//

#ifndef MINI_JVM_CLASSWRITER_H
#define MINI_JVM_CLASSWRITER_H
#include "Common.h"
#include <fstream>
namespace jvm {
class ClassFile;
class ClassWriter {
public:
  ClassWriter(const char* file_);

  ~ClassWriter();

  void WriteUInt1(u1 data);
  void WriteUInt2(u2 data);
  void WriteInt4(u4 data);
  void WriteUTF(const std::string &s);

public:
  ClassFile *classFile;
  std::ofstream out;
};
} // namespace jvm
#endif // MINI_JVM_CLASSWRITER_H
