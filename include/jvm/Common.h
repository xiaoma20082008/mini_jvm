//
// Created by machunxiao on 2020/4/6.
//

#ifndef MINI_JVM_COMMON_H
#define MINI_JVM_COMMON_H
#include <cstdint>

namespace jvm {

// region value of data length

#define u1 std::uint8_t
#define u2 std::uint16_t
#define u4 std::uint32_t

// endregion value of data length

// region value of access flags

#define ACC_PUBLIC      0x0001
#define ACC_FINAL       0x0010
#define ACC_SUPER       0x0020
#define ACC_INTERFACE   0x0200
#define ACC_ABSTRACT    0x0400
#define ACC_SYNTHETIC   0x1000
#define ACC_ANNOTATION  0x2000
#define ACC_ENUM        0x4000

// endregion value of access flags

// region Constant pool tags

#define CONSTANT_Utf8               1
#define CONSTANT_Integer            3
#define CONSTANT_Float              4
#define CONSTANT_Long               5
#define CONSTANT_Double             6
#define CONSTANT_Class              7
#define CONSTANT_String             8
#define CONSTANT_FieldRef           9
#define CONSTANT_MethodRef          10
#define CONSTANT_InterfaceMethodRef 11
#define CONSTANT_NameAndType        12
#define CONSTANT_MethodHandle       15
#define CONSTANT_MethodType         16
#define CONSTANT_Dynamic            17
#define CONSTANT_InvokeDynamic      18
#define CONSTANT_Module             19
#define CONSTANT_Package            20

// endregion Constant pool tags

// region MethodHandle type

#define REF_getField          1
#define REF_getStatic         2
#define REF_putField          3
#define REF_putStatic         4
#define REF_invokeVirtual     5
#define REF_invokeStatic      6
#define REF_invokeSpecial     7
#define REF_newInvokeSpecial  8
#define REF_invokeInterface   9

// endregion MethodHandle type

} // namespace jvm
#endif // MINI_JVM_COMMON_H
