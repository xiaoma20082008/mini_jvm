//
// Created by machunxiao on 2020/4/21.
//

#ifndef MINI_JVM_FILEIO_H
#define MINI_JVM_FILEIO_H
#include "fstream"

namespace jvm::io {

class FileReader {
public:
  explicit FileReader(const std::string& filepath_)
      : filepath(filepath_), in(filepath_, std::ios::in | std::ios::binary) {}

  ~FileReader() { in.close(); }

  u1 ReadUInt1() {
    u1 ch = 0;
    in.read(reinterpret_cast<char*>(&ch), sizeof(u1));
    return ch;
  }

  u2 ReadUInt2() {
    u1 ch1 = ReadUInt1();
    u1 ch2 = ReadUInt1();
    if ((ch1 | ch2) < 0) {
      throw EofException("read end of file");
    }
    u2 res = (ch1 << 8) + (ch2 << 0);
    return res;
  }

  u4 ReadInt4() {
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

  void ReadFully(u1* data_, u4 offset_, u4 length_) {
    for (u4 i = offset_; i < length_; ++i) {
      data_[i] = ReadUInt1();
    }
  }

  std::string ReadUTF() {
    u2 len = ReadUInt2();
    u1* data = new u1[len];
    ReadFully(data, 0, len);
    return std::string(reinterpret_cast<const char*>(data), len);
  }

private:
  std::string filepath;
  std::ifstream in;
};

class FileWriter {
public:
  explicit FileWriter(const std::string& filepath_)
      : filepath(filepath_), out(filepath_, std::ios::in | std::ios::binary) {}

  ~FileWriter() { out.close(); }

  void WriteUInt1(u1 data) {}

  void ReadUInt2(u2 data) {}

  void ReadInt4(u4 data) {}

  void ReadFully(u1* data_, u4 offset_, u4 length_) {}

  void ReadUTF(std::string data) {}

private:
  std::string filepath;
  std::ofstream out;
};

} // namespace jvm::io
#endif // MINI_JVM_FILEIO_H
