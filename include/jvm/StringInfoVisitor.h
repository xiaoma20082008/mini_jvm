//
// Created by machunxiao on 2020/4/7.
//

#ifndef MINI_JVM_STRINGINFOVISITOR_H
#define MINI_JVM_STRINGINFOVISITOR_H
#include "ConstantInfoVisitor.h"
#include <string>
namespace jvm {
class StringInfoVisitor : public ConstantInfoVisitor<std::string, int> {
public:
  std::string Visit(ConstantClassInfo *cp, int ident) override;
  std::string Visit(ConstantFieldRefInfo *cp, int ident) override;
  std::string Visit(ConstantMethodRefInfo *cp, int ident) override;
  std::string Visit(ConstantInterfaceMethodRefInfo *cp, int ident) override;
  std::string Visit(ConstantStringInfo *cp, int ident) override;
  std::string Visit(ConstantIntegerInfo *cp, int ident) override;
  std::string Visit(ConstantFloatInfo *cp, int ident) override;
  std::string Visit(ConstantLongInfo *cp, int ident) override;
  std::string Visit(ConstantDoubleInfo *cp, int ident) override;
  std::string Visit(ConstantNameAndTypeInfo *cp, int ident) override;
  std::string Visit(ConstantUtf8Info *cp, int ident) override;
  std::string Visit(ConstantMethodHandleInfo *cp, int ident) override;
  std::string Visit(ConstantMethodTypeInfo *cp, int ident) override;
  std::string Visit(ConstantDynamicInfo *cp, int ident) override;
  std::string Visit(ConstantInvokeDynamicInfo *cp, int ident) override;
  std::string Visit(ConstantModuleInfo *cp, int ident) override;
  std::string Visit(ConstantPackageInfo *cp, int ident) override;
};
} // namespace jvm
#endif // MINI_JVM_STRINGINFOVISITOR_H
