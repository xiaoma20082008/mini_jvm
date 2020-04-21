//
// Created by machunxiao on 2020/4/10.
//

#ifndef MINI_JVM_ACCESSFLAGS_H
#define MINI_JVM_ACCESSFLAGS_H
#include "Common.h"
#include <set>
namespace jvm {
enum Kind { Class_T, InnerClass_T, Field_T, Method_T };
class AccessFlags {
public:
  explicit AccessFlags(u2 flags_) : flags(flags_) {}

public:
  std::set<std::string> GetClassModifiers();
  std::set<std::string> GetClassFlags();
  std::set<std::string> GetInnerClassModifiers();
  std::set<std::string> GetInnerClassFlags();
  std::set<std::string> GetFieldClassModifiers();
  std::set<std::string> GetFieldClassFlags();
  std::set<std::string> GetMethodClassModifiers();
  std::set<std::string> GetMethodClassFlags();
  bool Is(u2 mask);

public:
  u2 flags;

private:
  static std::set<std::string> GetModifiers(u2 flag_, const u2* expect_flags,
                                            u2 expect_flags_count_, Kind t);

  static std::string FlagToModifier(u2 flag_, Kind t);

  static std::string FlagToName(u2 flag_, Kind t);

private:
  std::set<std::string> GetFlags(const u2* expect_flags, u2 expect_flags_count_,
                                 Kind t);

  constexpr static const u2 class_modifiers_count = 3;
  constexpr static const u2 class_flags_count = 9;
  constexpr static const u2 inner_class_modifiers_count = 6;
  constexpr static const u2 inner_class_flags_count = 11;
  constexpr static const u2 field_modifiers_count = 7;
  constexpr static const u2 field_flags_count = 9;
  constexpr static const u2 method_modifiers_count = 9;
  constexpr static const u2 method_flags_count = 12;

  constexpr static const u2 class_modifiers[class_modifiers_count] = {
      ACC_PUBLIC, ACC_FINAL, ACC_ABSTRACT};
  constexpr static const u2 class_flags[class_flags_count] = {
      ACC_PUBLIC,    ACC_FINAL,      ACC_SUPER, ACC_INTERFACE, ACC_ABSTRACT,
      ACC_SYNTHETIC, ACC_ANNOTATION, ACC_ENUM,  ACC_MODULE};
  constexpr static const u2 inner_class_modifiers[inner_class_modifiers_count] =
      {ACC_PUBLIC, ACC_PRIVATE, ACC_PROTECTED,
       ACC_STATIC, ACC_FINAL,   ACC_ABSTRACT};
  constexpr static const u2 inner_class_flags[inner_class_flags_count] = {
      ACC_PUBLIC,    ACC_PRIVATE,    ACC_PROTECTED, ACC_STATIC,
      ACC_FINAL,     ACC_SUPER,      ACC_INTERFACE, ACC_ABSTRACT,
      ACC_SYNTHETIC, ACC_ANNOTATION, ACC_ENUM};
  constexpr static const u2 field_modifiers[field_modifiers_count] = {
      ACC_PUBLIC, ACC_PRIVATE,  ACC_PROTECTED, ACC_STATIC,
      ACC_FINAL,  ACC_VOLATILE, ACC_TRANSIENT};
  constexpr static const u2 field_flags[field_flags_count] = {
      ACC_PUBLIC,   ACC_PRIVATE,   ACC_PROTECTED, ACC_STATIC, ACC_FINAL,
      ACC_VOLATILE, ACC_TRANSIENT, ACC_SYNTHETIC, ACC_ENUM};
  constexpr static const u2 method_modifiers[method_modifiers_count] = {
      ACC_PUBLIC,       ACC_PRIVATE, ACC_PROTECTED, ACC_STATIC, ACC_FINAL,
      ACC_SYNCHRONIZED, ACC_NATIVE,  ACC_ABSTRACT,  ACC_STRICT};
  constexpr static const u2 method_flags[method_flags_count] = {
      ACC_PUBLIC, ACC_PRIVATE,      ACC_PROTECTED, ACC_STATIC,
      ACC_FINAL,  ACC_SYNCHRONIZED, ACC_BRIDGE,    ACC_VARARGS,
      ACC_NATIVE, ACC_ABSTRACT,     ACC_STRICT,    ACC_SYNTHETIC};
};
} // namespace jvm
#endif // MINI_JVM_ACCESSFLAGS_H
