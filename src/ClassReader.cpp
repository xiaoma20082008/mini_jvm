//
// Created by machunxiao on 2020/4/6.
//

#include "ClassReader.h"
#include "Attribute.h"
#include "ClassFile.h"
#include "Exception.h"
namespace jvm {

ClassReader::ClassReader(const char *file_, AttributeFactory *factory_) {
  in.open(file_, std::ios::in | std::ios::binary);
  if (!in.is_open()) {
    std::string msg;
    msg.append("fail to open file:");
    msg.append(file_);
    throw IOException(msg);
  }

#ifdef PRINT_DEBUG
  in.seekg(0, std::ios::end);
  auto len = in.tellg();
  file_size = len;

  in.seekg(0, std::ios::beg);
#endif

  factory = factory_;
}

ClassReader::~ClassReader() {
  in.close();
  delete factory;
}

void ClassReader::ReadFully(u1 *data_, u4 offset_, u4 length_) {
  for (u4 i = offset_; i < length_; ++i) {
    data_[i] = ReadUInt1();
  }
}

u4 ClassReader::ReadInt4() {
  u1 ch1 = ReadUInt1();
  u1 ch2 = ReadUInt1();
  u1 ch3 = ReadUInt1();
  u1 ch4 = ReadUInt1();
  if ((ch1 | ch2 | ch3 | ch4) < 0) {
    throw EofException("read end of file");
  }
  u4 res = ((ch1 << 24) + (ch2 << 16) + (ch3 << 8) + (ch4 << 0));
  return res;
}

u2 ClassReader::ReadUInt2() {
  u1 ch1 = ReadUInt1();
  u1 ch2 = ReadUInt1();
  if ((ch1 | ch2) < 0) {
    throw EofException("read end of file");
  }
  u2 res = (ch1 << 8) + (ch2 << 0);
  return res;
}

u1 ClassReader::ReadUInt1() {
  u1 ch = 0;
  in.read(reinterpret_cast<char *>(&ch), sizeof(u1));
#ifdef PRINT_DEBUG
  index++;
#endif
  return ch;
}

std::string ClassReader::ReadUTF() {
  u2 len = ReadUInt2();
  u1 *data = new u1[len];
  ReadFully(data, 0, len);
  return std::string(reinterpret_cast<const char *>(data), len);
}

Attribute *ClassReader::ReadAttribute() {
  return factory->CreateAttribute(this);
}

ClassFile *ClassReader::GetClassFile() { return factory->class_file; }

} // namespace jvm