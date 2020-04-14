//
// Created by machunxiao on 2020/4/10.
//
#include "AccessFlags.h"
#include <array>
namespace jvm {
AccessFlags::AccessFlags(u2 flags_) { flags = flags_; }

std::set<std::string> AccessFlags::GetClassModifiers() {
  std::set<std::string> s;
  u2 f = ((flags & ACC_INTERFACE) != 0 ? flags & ~ACC_ABSTRACT : flags);
  return GetModifiers(f, class_modifiers, class_modifiers_count, Kind::Class_T);
}
std::set<std::string> AccessFlags::GetClassFlags() {
  std::set<std::string> s;
  return s;
}
std::set<std::string> AccessFlags::GetInnerClassModifiers() {
  std::set<std::string> s;
  return s;
}
std::set<std::string> AccessFlags::GetInnerClassFlags() {
  std::set<std::string> s;
  return s;
}
std::set<std::string> AccessFlags::GetFieldClassModifiers() {
  std::set<std::string> s;
  return s;
}
std::set<std::string> AccessFlags::GetFieldClassFlags() {
  std::set<std::string> s;
  return s;
}
std::set<std::string> AccessFlags::GetMethodClassModifiers() {
  std::set<std::string> s;
  return s;
}
std::set<std::string> AccessFlags::GetMethodClassFlags() {
  std::set<std::string> s;
  return s;
}

bool AccessFlags::Is(u2 mask) { return (flags & mask) != 0; }

std::set<std::string> AccessFlags::GetFlags(const u2 *expect_flags,
                                            u2 expect_flags_count_,
                                            jvm::Kind t) {
  std::set<std::string> s;
  u2 f = flags;
  for (u2 i = 0; i < expect_flags_count_; ++i) {
    u2 m = expect_flags[i];
    if ((flags & m) != 0) {
      s.insert(FlagToName(m, t));
      f = f & ~m;
    }
  }
  /*
  todo
  while (f != 0) {
  }
  */
  return s;
}
std::set<std::string> AccessFlags::GetModifiers(u2 flag_,
                                                const u2 *expect_flags,
                                                u2 expect_flags_count_,
                                                jvm::Kind t) {
  std::set<std::string> s;
  for (u2 i = 0; i < expect_flags_count_; ++i) {
    u2 m = expect_flags[i];
    if ((flag_ & m) != 0) {
      s.insert(FlagToModifier(m, t));
    }
  }
  return s;
}
std::string AccessFlags::FlagToName(u2 flag_, jvm::Kind t) {
  switch (flag_) {
  case ACC_PUBLIC:
    return "ACC_PUBLIC";
  case ACC_PRIVATE:
    return "ACC_PRIVATE";
  case ACC_PROTECTED:
    return "ACC_PROTECTED";
  case ACC_STATIC:
    return "ACC_STATIC";
  case ACC_FINAL:
    return "ACC_FINAL";
  case 0x20:
    return (t == Kind::Class_T ? "ACC_SUPER" : "ACC_SYNCHRONIZED");
  case 0x40:
    return (t == Kind::Field_T ? "ACC_VOLATILE" : "ACC_BRIDGE");
  case 0x80:
    return (t == Kind::Field_T ? "ACC_TRANSIENT" : "ACC_VARARGS");
  case ACC_NATIVE:
    return "ACC_NATIVE";
  case ACC_INTERFACE:
    return "ACC_INTERFACE";
  case ACC_ABSTRACT:
    return "ACC_ABSTRACT";
  case ACC_STRICT:
    return "ACC_STRICT";
  case ACC_SYNTHETIC:
    return "ACC_SYNTHETIC";
  case ACC_ANNOTATION:
    return "ACC_ANNOTATION";
  case ACC_ENUM:
    return "ACC_ENUM";
  case 0x8000:
    return (t == Kind::Class_T ? "ACC_MODULE" : "ACC_MANDATED");
  default:
    return "";
  }
}
std::string AccessFlags::FlagToModifier(u2 flag_, jvm::Kind t) {
  switch (flag_) {
  case ACC_PUBLIC:
    return "public";
  case ACC_PRIVATE:
    return "private";
  case ACC_PROTECTED:
    return "protected";
  case ACC_STATIC:
    return "static";
  case ACC_FINAL:
    return "final";
  case ACC_SYNCHRONIZED:
    return "synchronized";
  case 0x80:
    return (t == Kind::Field_T ? "transient" : "");
  case ACC_VOLATILE:
    return "volatile";
  case ACC_NATIVE:
    return "native";
  case ACC_ABSTRACT:
    return "abstract";
  case ACC_STRICT:
    return "strictfp";
  case ACC_MANDATED:
    return "mandated";
  default:
    return "";
  }
}
} // namespace jvm