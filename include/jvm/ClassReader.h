//
// Created by machunxiao on 2020/4/6.
//

#ifndef MINI_JVM_CLASSREADER_H
#define MINI_JVM_CLASSREADER_H

#include "Common.h"

namespace jvm {
class ClassReader {
public:
  u1 ReadUInt1();
  u2 ReadUInt2();
  u4 ReadUInt4();
};
} // namespace jvm
#endif // MINI_JVM_CLASSREADER_H
