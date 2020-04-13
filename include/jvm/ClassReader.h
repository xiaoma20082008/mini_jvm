//
// Created by machunxiao on 2020/4/6.
//

#ifndef MINI_JVM_CLASSREADER_H
#define MINI_JVM_CLASSREADER_H

#include "AccessFlags.h"
#include "Common.h"
#include <fstream>

namespace jvm {
class Attribute;
class AttributeFactory;
class ClassFile;
class ClassReader {
public:
  ClassReader(const char *file_, AttributeFactory *factory_);

  ~ClassReader();
  [[nodiscard]] u1 ReadUInt1();
  [[nodiscard]] u2 ReadUInt2();
  [[nodiscard]] u4 ReadInt4();
  void ReadFully(u1 *data_, u4 offset_, u4 length_);
  [[nodiscard]] std::string ReadUTF();
  [[nodiscard]] Attribute *ReadAttribute();
  ClassFile *GetClassFile();

#ifdef PRINT_DEBUG
public:
  u4 file_size = 0;
  u4 index = 0;
#endif

private:
  AttributeFactory *factory = nullptr;
  std::ifstream in;
};
} // namespace jvm
#endif // MINI_JVM_CLASSREADER_H
