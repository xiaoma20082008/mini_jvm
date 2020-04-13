//
// Created by machunxiao on 2020/4/6.
//

#ifndef MINI_JVM_CONSTANTPOOL_H
#define MINI_JVM_CONSTANTPOOL_H

#include "Exception.h"
#include <memory>
#include <utility>
#include <vector>
namespace jvm {

class ConstantPool;
class ClassReader;
class ClassWriter;

template <typename R, typename D> class ConstantInfoVisitor;
struct CpInfo {
public:
  CpInfo(u1 tag_);
  CpInfo(ConstantPool *cp_, u1 tag_);
  virtual ~CpInfo();
  virtual u4 Size();
  virtual u4 Length() = 0;
  virtual u1 GetTag() final;
  template <typename R, typename D>
  R Accept(ConstantInfoVisitor<R, D> *v, D *data);

public:
  u1 tag = 0;
  ConstantPool *cp = nullptr;
};
struct ConstantClassInfo : public CpInfo {
  ConstantClassInfo(ConstantPool *cp_, ClassReader *reader, u1 tag_);
  ConstantClassInfo(ConstantPool *cp_, u1 tag_, u2 name_index_);
  u4 Length() override;
  template <typename R, typename D>
  R Accept(ConstantInfoVisitor<R, D> *v, D *data);
  std::string GetName();
  u2 name_index;
};
struct CpRef : public CpInfo {
  u2 class_index = 0;
  u2 name_and_type_index = 0;

  CpRef(ConstantPool *cp_, ClassReader *reader, u1 tag_);
  CpRef(ConstantPool *cp_, u1 tag_, u2 class_index_, u2 name_and_type_index_);
  u4 Length() final;

  template <typename R, typename D>
  R Accept(ConstantInfoVisitor<R, D> *v, D *data);
};
struct ConstantFieldRefInfo : public CpRef {
  ConstantFieldRefInfo(ConstantPool *cp_, ClassReader *reader_, u1 tag_);
  ConstantFieldRefInfo(ConstantPool *cp_, u1 tag_, u2 class_index_,
                       u2 name_and_type_index_);
  template <typename R, typename D>
  R Accept(ConstantInfoVisitor<R, D> *v, D *data);
};
struct ConstantMethodRefInfo : public CpRef {
  ConstantMethodRefInfo(ConstantPool *cp_, ClassReader *reader_, u1 tag_);
  ConstantMethodRefInfo(ConstantPool *cp_, u1 tag_, u2 class_index_,
                        u2 name_and_type_index_);
  template <typename R, typename D>
  R Accept(ConstantInfoVisitor<R, D> *v, D *data);
};
struct ConstantInterfaceMethodRefInfo : public CpRef {
  ConstantInterfaceMethodRefInfo(ConstantPool *cp_, ClassReader *reader_,
                                 u1 tag_);
  ConstantInterfaceMethodRefInfo(ConstantPool *cp_, u1 tag_, u2 class_index_,
                                 u2 name_and_type_index_);

  template <typename R, typename D>
  R Accept(ConstantInfoVisitor<R, D> *v, D *data);
};
struct ConstantStringInfo : public CpInfo {
  ConstantStringInfo(ConstantPool *cp_, ClassReader *reader, u1 tag_);
  ConstantStringInfo(ConstantPool *cp_, u1 tag_, u2 string_index_);
  u4 Length() override;

  template <typename R, typename D>
  R Accept(ConstantInfoVisitor<R, D> *v, D *data);

  u2 string_index = 0;
};
struct ConstantIntegerInfo : public CpInfo {
  ConstantIntegerInfo(ConstantPool *cp_, ClassReader *reader, u1 tag_);
  ConstantIntegerInfo(ConstantPool *cp_, u1 tag_, u4 bytes_);
  u4 Length() override;

  template <typename R, typename D>
  R Accept(ConstantInfoVisitor<R, D> *v, D *data);

  u4 bytes = 0;
};
struct ConstantFloatInfo : public CpInfo {
  ConstantFloatInfo(ConstantPool *cp_, ClassReader *reader, u1 tag_);
  ConstantFloatInfo(ConstantPool *cp_, u1 tag_, u4 bytes_);
  u4 Length() override;

  template <typename R, typename D>
  R Accept(ConstantInfoVisitor<R, D> *v, D *data);
  u4 bytes = 0;
};
struct ConstantLongInfo : public CpInfo {

  ConstantLongInfo(ConstantPool *cp_, ClassReader *reader, u1 tag_);
  ConstantLongInfo(ConstantPool *cp_, u1 tag_, u4 high_bytes_, u4 low_bytes_);
  u4 Size() override;
  u4 Length() override;

  template <typename R, typename D>
  R Accept(ConstantInfoVisitor<R, D> *v, D *data);

  u4 high_bytes = 0;
  u4 low_bytes = 0;
};
struct ConstantDoubleInfo : public CpInfo {

  ConstantDoubleInfo(ConstantPool *cp_, ClassReader *reader, u1 tag_);
  ConstantDoubleInfo(ConstantPool *cp_, u1 tag_, u4 high_bytes_, u4 low_bytes_);
  u4 Size();
  u4 Length();
  template <typename R, typename D>
  R Accept(ConstantInfoVisitor<R, D> *v, D *data);

  u4 high_bytes = 0;
  u4 low_bytes = 0;
};
struct ConstantNameAndTypeInfo : public CpInfo {

  ConstantNameAndTypeInfo(ConstantPool *cp_, ClassReader *reader, u1 tag_);
  ConstantNameAndTypeInfo(ConstantPool *cp_, u1 tag_, u2 name_index_,
                          u2 descriptor_index_);
  u4 Length() override;

  template <typename R, typename D>
  R Accept(ConstantInfoVisitor<R, D> *v, D *data);

  u2 name_index = 0;
  u2 descriptor_index = 0;
};
struct ConstantUtf8Info : public CpInfo {
public:
  ConstantUtf8Info(ConstantPool *cp_, ClassReader *reader, u1 tag_);
  u4 Length();
  template <typename R, typename D>
  R Accept(ConstantInfoVisitor<R, D> *v, D *data);

public:
  std::string value;
};
struct ConstantMethodHandleInfo : public CpInfo {

  ConstantMethodHandleInfo(ConstantPool *cp_, ClassReader *reader, u1 tag_);
  ConstantMethodHandleInfo(ConstantPool *cp_, u1 tag_, u1 reference_kind_,
                           u2 reference_index_);
  u4 Length() override;

  template <typename R, typename D>
  R Accept(ConstantInfoVisitor<R, D> *v, D *data);

  u1 reference_kind = 0;
  u2 reference_index = 0;
};
struct ConstantMethodTypeInfo : public CpInfo {
  ConstantMethodTypeInfo(ConstantPool *cp_, ClassReader *reader, u1 tag_);
  ConstantMethodTypeInfo(ConstantPool *cp_, u1 tag_, u2 descriptor_index_);
  u4 Length() override;

  template <typename R, typename D>
  R Accept(ConstantInfoVisitor<R, D> *v, D *data);
  u2 descriptor_index = 0;
};
struct ConstantDynamicInfo : public CpInfo {

  ConstantDynamicInfo(ConstantPool *cp_, ClassReader *reader, u1 tag_);
  ConstantDynamicInfo(ConstantPool *cp_, u1 tag_,
                      u2 bootstrap_method_attr_index_, u2 name_and_type_index_);
  u4 Length() override;

  template <typename R, typename D>
  R Accept(ConstantInfoVisitor<R, D> *v, D *data);

  u2 bootstrap_method_attr_index = 0;
  u2 name_and_type_index = 0;
};
struct ConstantInvokeDynamicInfo : public CpInfo {

  ConstantInvokeDynamicInfo(ConstantPool *cp_, ClassReader *reader, u1 tag_);
  ConstantInvokeDynamicInfo(ConstantPool *cp_, u1 tag_,
                            u2 bootstrap_method_attr_index_,
                            u2 name_and_type_index_);
  u4 Length() override;

  template <typename R, typename D>
  R Accept(ConstantInfoVisitor<R, D> *v, D *data);

  u2 bootstrap_method_attr_index = 0;
  u2 name_and_type_index = 0;
};
struct ConstantModuleInfo : public CpInfo {
  ConstantModuleInfo(ConstantPool *cp_, ClassReader *reader, u1 tag_);
  ConstantModuleInfo(ConstantPool *cp_, u1 tag_, u2 name_index_);
  u4 Length() override;

  template <typename R, typename D>
  R Accept(ConstantInfoVisitor<R, D> *v, D *data);

  u2 name_index = 0;
};
struct ConstantPackageInfo : public CpInfo {
  ConstantPackageInfo(ConstantPool *cp_, ClassReader *reader, u1 tag_);
  ConstantPackageInfo(ConstantPool *cp_, u1 tag_, u2 name_index_);
  u4 Length() override;

  template <typename R, typename D>
  R Accept(ConstantInfoVisitor<R, D> *v, D *data);

  u2 name_index = 0;
};

class ConstantPool {
public:
  ConstantPool(ClassReader *reader_);
  ConstantPool(u2 constant_pool_count_, std::vector<CpInfo *> pool);
  ~ConstantPool();
  u4 Size();
  u4 Length();
  std::string GetUTF8Value(u4 index);
  u4 GetUTF8Index(const std::string &value_);
  ConstantUtf8Info *GetUTF8Info(u4 index);
  ConstantClassInfo *GetClassInfo(u4 index);
  ConstantModuleInfo *GetModuleInfo(u4 index);
  ConstantNameAndTypeInfo *GetNameAndTypeInfo(u4 index);
  ConstantPackageInfo *GetPackageInfo(u4 index);
  CpInfo *Get(u4 index, u1 expected_type);

private:
  CpInfo *Get(u4 index);

public:
  std::vector<CpInfo *> pool;

private:
  u2 constant_pool_count;
};

} // namespace jvm
#endif // MINI_JVM_CONSTANTPOOL_H
