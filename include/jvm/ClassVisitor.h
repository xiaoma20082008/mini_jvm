//
// Created by machunxiao on 2020/4/6.
//

#ifndef MINI_JVM_CLASSVISITOR_H
#define MINI_JVM_CLASSVISITOR_H
namespace jvm {

struct CpInfo;
struct ConstantClassInfo;
struct CpRef;
struct ConstantFieldRefInfo;
struct ConstantMethodRefInfo;
struct ConstantInterfaceMethodRefInfo;
struct ConstantStringInfo;
struct ConstantIntegerInfo;
struct ConstantFloatInfo;
struct ConstantLongInfo;
struct ConstantDoubleInfo;
struct ConstantNameAndTypeInfo;
struct ConstantUtf8Info;
struct ConstantMethodHandleInfo;
struct ConstantMethodTypeInfo;
struct ConstantDynamicInfo;
struct ConstantInvokeDynamicInfo;
struct ConstantModuleInfo;
struct ConstantPackageInfo;

template <typename R, typename P> class Visitor {
  virtual R Visit(CpInfo *cp, P p) {}
  virtual R Visit(ConstantClassInfo *cp, P p) {}
  virtual R Visit(CpRef *cp, P p) {}
  virtual R Visit(ConstantFieldRefInfo *cp, P p) {}
  virtual R Visit(ConstantMethodRefInfo *cp, P p) {}
  virtual R Visit(ConstantInterfaceMethodRefInfo *cp, P p) {}
  virtual R Visit(ConstantStringInfo *cp, P p) {}
  virtual R Visit(ConstantIntegerInfo *cp, P p) {}
  virtual R Visit(ConstantFloatInfo *cp, P p) {}
  virtual R Visit(ConstantLongInfo *cp, P p) {}
  virtual R Visit(ConstantDoubleInfo *cp, P p) {}
  virtual R Visit(ConstantNameAndTypeInfo *cp, P p) {}
  virtual R Visit(ConstantUtf8Info *cp, P p) {}
  virtual R Visit(ConstantMethodHandleInfo *cp, P p) {}
  virtual R Visit(ConstantMethodTypeInfo *cp, P p) {}
  virtual R Visit(ConstantDynamicInfo *cp, P p) {}
  virtual R Visit(ConstantInvokeDynamicInfo *cp, P p) {}
  virtual R Visit(ConstantModuleInfo *cp, P p) {}
  virtual R Visit(ConstantPackageInfo *cp, P p) {}
};

} // namespace jvm
#endif // MINI_JVM_CLASSVISITOR_H
