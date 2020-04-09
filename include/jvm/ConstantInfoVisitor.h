//
// Created by machunxiao on 2020/4/6.
//

#ifndef MINI_JVM_CONSTANTINFOVISITOR_H
#define MINI_JVM_CONSTANTINFOVISITOR_H
namespace jvm {

struct ConstantClassInfo;
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

template <typename R, typename P> class ConstantInfoVisitor {
public:
  virtual R Visit(ConstantClassInfo *cp, P p) = 0;
  virtual R Visit(ConstantFieldRefInfo *cp, P p) = 0;
  virtual R Visit(ConstantMethodRefInfo *cp, P p) = 0;
  virtual R Visit(ConstantInterfaceMethodRefInfo *cp, P p) = 0;
  virtual R Visit(ConstantStringInfo *cp, P p) = 0;
  virtual R Visit(ConstantIntegerInfo *cp, P p) = 0;
  virtual R Visit(ConstantFloatInfo *cp, P p) = 0;
  virtual R Visit(ConstantLongInfo *cp, P p) = 0;
  virtual R Visit(ConstantDoubleInfo *cp, P p) = 0;
  virtual R Visit(ConstantNameAndTypeInfo *cp, P p) = 0;
  virtual R Visit(ConstantUtf8Info *cp, P p) = 0;
  virtual R Visit(ConstantMethodHandleInfo *cp, P p) = 0;
  virtual R Visit(ConstantMethodTypeInfo *cp, P p) = 0;
  virtual R Visit(ConstantDynamicInfo *cp, P p) = 0;
  virtual R Visit(ConstantInvokeDynamicInfo *cp, P p) = 0;
  virtual R Visit(ConstantModuleInfo *cp, P p) = 0;
  virtual R Visit(ConstantPackageInfo *cp, P p) = 0;
};

} // namespace jvm
#endif // MINI_JVM_CONSTANTINFOVISITOR_H
