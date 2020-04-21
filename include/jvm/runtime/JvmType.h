//
// Created by machunxiao on 2020/4/16.
//

#ifndef MINI_JVM_JVMTYPE_H
#define MINI_JVM_JVMTYPE_H

#include "Common.h"

namespace jvm {

#define BASE_OF_JVM_TYPE :public JvmType
#define BASE_OF_PRIMITIVE_TYPE :public PrimitiveType
#define BASE_OF_REFERENCE_TYPE :public ReferenceType

///
/// https://docs.oracle.com/javase/specs/jvms/se14/html/jvms-2.html#jvms-2.2
///

struct JvmType {
  virtual ~JvmType() = default;
};

struct PrimitiveType BASE_OF_JVM_TYPE {};
struct ReferenceType BASE_OF_JVM_TYPE {
  // offset in jvm heap
  uint64_t offset = 0;
};

struct BooleanType BASE_OF_PRIMITIVE_TYPE {
  int32_t value = 0;
};
struct ByteType BASE_OF_PRIMITIVE_TYPE {
  int8_t value = 0;
};
struct ShortType BASE_OF_PRIMITIVE_TYPE {
  int16_t value = 0;
};
struct IntType BASE_OF_PRIMITIVE_TYPE {
  int32_t value = 0;
};
struct LongType BASE_OF_PRIMITIVE_TYPE {
  int64_t value = 0L;
};
struct FloatType BASE_OF_PRIMITIVE_TYPE {
  float value = 0.0f;
};
struct DoubleType BASE_OF_PRIMITIVE_TYPE {
  double value = 0.0;
};

struct ClassType BASE_OF_REFERENCE_TYPE {};
struct ArrayType BASE_OF_REFERENCE_TYPE {};
struct InterfaceType BASE_OF_REFERENCE_TYPE {};

} // namespace jvm
#endif // MINI_JVM_JVMTYPE_H
