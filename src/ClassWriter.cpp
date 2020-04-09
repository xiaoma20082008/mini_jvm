//
// Created by machunxiao on 2020/4/6.
//

#include "ClassWriter.h"
#include "ClassFile.h"
namespace jvm {
ClassWriter::ClassWriter(const char *file_) {
  out.open(file_, std::ios::in | std::ios::binary);
  if (!out.is_open()) {
    std::string msg;
    msg.append("fail to open file:");
    msg.append(file_);
    throw IOException(msg);
  }
}
ClassWriter::~ClassWriter() { out.close(); }
void ClassWriter::WriteUInt1(u1 data) {}
void ClassWriter::WriteUInt2(u2 data) {}
void ClassWriter::WriteInt4(u4 data) {}
void ClassWriter::WriteUTF(const std::string &s) {}
} // namespace jvm