//
// Created by machunxiao on 2020/4/21.
//

#ifndef MINI_JVM_CONSTANTPOOL2_H
#define MINI_JVM_CONSTANTPOOL2_H
namespace jvm::cf {

struct CpInfo {
public:
  CpInfo(u1 tag_) : tag(tag_) {}
  virtual ~CpInfo() = default;
  u1 tag = 0;
};

struct ConstantClassInfo : public CpInfo {
  ConstantClassInfo(u1 tag_, u2 name_index_)
      : CpInfo(tag_), name_index(name_index_) {}
  u2 name_index;
};
struct CpRef : public CpInfo {

  CpRef(u1 tag_, u2 class_index_, u2 name_and_type_index_);

  u2 class_index = 0;
  u2 name_and_type_index = 0;
};
struct ConstantFieldRefInfo : public CpRef {};
struct ConstantMethodRefInfo : public CpRef {};
struct ConstantInterfaceMethodRefInfo : public CpRef {};
struct ConstantStringInfo : public CpInfo {
  ConstantStringInfo(u1 tag_, u2 string_index_);
  u2 string_index = 0;
};
struct ConstantIntegerInfo : public CpInfo {
  ConstantIntegerInfo(u1 tag_, u4 value_) : CpInfo(tag_), value(value_) {}
  u4 value = 0;
};
struct ConstantFloatInfo : public CpInfo {
  ConstantFloatInfo(u1 tag_, u4 bytes_);

  u4 bytes = 0;
};
struct ConstantLongInfo : public CpInfo {

  ConstantLongInfo(u1 tag_, u4 high_bytes_, u4 low_bytes_);

  u4 high_bytes = 0;
  u4 low_bytes = 0;
};
struct ConstantDoubleInfo : public CpInfo {

  ConstantDoubleInfo(u1 tag_, u4 high_bytes_, u4 low_bytes_);

  u4 high_bytes = 0;
  u4 low_bytes = 0;
};
struct ConstantNameAndTypeInfo : public CpInfo {

  ConstantNameAndTypeInfo(u1 tag_, u2 name_index_, u2 descriptor_index_);

  u2 name_index = 0;
  u2 descriptor_index = 0;
};
struct ConstantUtf8Info : public CpInfo {
  ConstantUtf8Info(u1 tag_, std::string value_);

  std::string value;
};
struct ConstantMethodHandleInfo : public CpInfo {

  ConstantMethodHandleInfo(u1 tag_, u1 reference_kind_, u2 reference_index_);

  u1 reference_kind = 0;
  u2 reference_index = 0;
};
struct ConstantMethodTypeInfo : public CpInfo {
  ConstantMethodTypeInfo(u1 tag_, u2 descriptor_index_);

  u2 descriptor_index = 0;
};
struct ConstantDynamicInfo : public CpInfo {

  ConstantDynamicInfo(u1 tag_, u2 bootstrap_method_attr_index_,
                      u2 name_and_type_index_);

  u2 bootstrap_method_attr_index = 0;
  u2 name_and_type_index = 0;
};
struct ConstantInvokeDynamicInfo : public CpInfo {

  ConstantInvokeDynamicInfo(u1 tag_, u2 bootstrap_method_attr_index_,
                            u2 name_and_type_index_);

  u2 bootstrap_method_attr_index = 0;
  u2 name_and_type_index = 0;
};
struct ConstantModuleInfo : public CpInfo {
  ConstantModuleInfo(u1 tag_, u2 name_index_);
  u2 name_index = 0;
};
struct ConstantPackageInfo : public CpInfo {
  ConstantPackageInfo(u1 tag_, u2 name_index_);
  u2 name_index = 0;
};

class ConstantPool {
public:
  ConstantPool(u2 constant_pool_count_, std::vector<CpInfo*> pool);
  ~ConstantPool();
  u4 Size();
  u4 Length();
  std::string GetUTF8Value(u4 index);
  u4 GetUTF8Index(const std::string& value_);
  ConstantUtf8Info* GetUTF8Info(u4 index);
  ConstantClassInfo* GetClassInfo(u4 index);
  ConstantModuleInfo* GetModuleInfo(u4 index);
  ConstantNameAndTypeInfo* GetNameAndTypeInfo(u4 index);
  ConstantPackageInfo* GetPackageInfo(u4 index);
  CpInfo* Get(u4 index, u1 expected_type);

private:
  CpInfo* Get(u4 index);

private:
  CpInfo** pool;
  u2 constant_pool_count;
};
} // namespace jvm::cf
#endif // MINI_JVM_CONSTANTPOOL2_H
