//
// Created by machunxiao on 2020/4/6.
//

#ifndef MINI_JVM_COMMON_H
#define MINI_JVM_COMMON_H
#include <cstdint>
#include <string>

namespace jvm {

#define TO_STRING() std::string ToString();

#define FOR_EACH(item, length) for (auto item = 0; item < length; ++item)

#define DEL_PTR_ARRAY(p, len)                                                  \
  for (auto item = 0; item < len; ++item) {                                    \
    delete p[item];                                                            \
  }                                                                            \
  delete[] p

// region value of data length

// A class file consists of a stream of 8-bit bytes. 16-bit and 32-bit
// quantities are constructed by reading in two and four consecutive 8-bit
// bytes, respectively. Multibyte data items are always stored in big-endian
// order, where the high bytes come first. This chapter defines the data types
// u1, u2, and u4 to represent an unsigned one-, two-, or four-byte quantity,
// respectively.
#define u1 std::uint8_t
#define u2 std::uint16_t
#define u4 std::int32_t

// endregion value of data length

// region value of access flags

// class, inner, field, method
#define ACC_PUBLIC         0x0001

//        inner, field, method
#define ACC_PRIVATE        0x0002

//        inner, field, method
#define ACC_PROTECTED      0x0004

//        inner, field, method
#define ACC_STATIC         0x0008

// class, inner, field, method
#define ACC_FINAL          0x0010

// class
#define ACC_SUPER          0x0020

//                      method
#define ACC_SYNCHRONIZED   0x0020

//               field
#define ACC_VOLATILE       0x0040

//                      method
#define ACC_BRIDGE         0x0040

//               field
#define ACC_TRANSIENT      0x0080

//                      method
#define ACC_VARARGS        0x0080

//                      method
#define ACC_NATIVE         0x0100

// class, inner
#define ACC_INTERFACE      0x0200

// class, inner,        method
#define ACC_ABSTRACT       0x0400

//                      method
#define ACC_STRICT         0x0800

// class, inner, field, method
#define ACC_SYNTHETIC      0x1000

// class, inner
#define ACC_ANNOTATION     0x2000

// class, inner, field
#define ACC_ENUM           0x4000

//                          method parameter
#define ACC_MANDATED       0x8000

// class
#define ACC_MODULE         0x8000

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


// region Attribute

#define AnnotationDefault         "AnnotationDefault"
#define BootstrapMethods          "BootstrapMethods"
#define CharacterRangeTable       "CharacterRangeTable"
#define Code                      "Code"
#define ConstantValue             "ConstantValue"
#define CompilationID             "CompilationID"
#define Deprecated                "Deprecated"
#define EnclosingMethod           "EnclosingMethod"
#define Exceptions                "Exceptions"
#define InnerClasses              "InnerClasses"
#define LineNumberTable           "LineNumberTable"
#define LocalVariableTable        "LocalVariableTable"
#define LocalVariableTypeTable    "LocalVariableTypeTable"
#define MethodParameters          "MethodParameters"
#define Module                    "Module"
#define ModuleHashes              "ModuleHashes"
#define ModuleMainClass           "ModuleMainClass"
#define ModulePackages            "ModulePackages"
#define ModuleResolution          "ModuleResolution"
#define ModuleTarget              "ModuleTarget"
#define NestHost                  "NestHost"
#define NestMembers               "NestMembers"
#define Record                    "Record"
#define RuntimeVisibleAnnotations             "RuntimeVisibleAnnotations"
#define RuntimeInvisibleAnnotations           "RuntimeInvisibleAnnotations"
#define RuntimeVisibleParameterAnnotations    "RuntimeVisibleParameterAnnotations"
#define RuntimeInvisibleParameterAnnotations  "RuntimeInvisibleParameterAnnotations"
#define RuntimeVisibleTypeAnnotations         "RuntimeVisibleTypeAnnotations"
#define RuntimeInvisibleTypeAnnotations       "RuntimeInvisibleTypeAnnotations"
#define Signature                 "Signature"
#define SourceDebugExtension      "SourceDebugExtension"
#define SourceFile                "SourceFile"
#define SourceID                  "SourceID"
#define StackMap                  "StackMap"
#define StackMapTable             "StackMapTable"
#define Synthetic                 "Synthetic"

// endregion Attribute

} // namespace jvm
#endif // MINI_JVM_COMMON_H
