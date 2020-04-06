//
// Created by machunxiao on 2020/4/6.
//

#include "ConstantPool.h"

namespace jvm {

ConstantPool::ConstantPool(jvm::ClassReader *reader_) {
  u2 count = reader_->ReadUInt2();
  for (int i = 0; i < count; ++i) {
    u1 tag = reader_->ReadUInt1();
    switch (tag) {
    case CONSTANT_Utf8:
      pool.push_back(new ConstantUtf8Info(this, reader_));
      break;
    case CONSTANT_Integer:
      pool.push_back(new ConstantIntegerInfo(this, reader_));
      break;
    case CONSTANT_Float:
      pool.push_back(new ConstantFloatInfo(this, reader_));
      break;
    case CONSTANT_Long:
      pool.push_back(new ConstantLongInfo(this, reader_));
      break;
    case CONSTANT_Double:
      pool.push_back(new ConstantDoubleInfo(this, reader_));
      break;
    case CONSTANT_Class:
      pool.push_back(new ConstantClassInfo(this, reader_));
      break;
    case CONSTANT_String:
      pool.push_back(new ConstantStringInfo(this, reader_));
      break;
    case CONSTANT_FieldRef:
      pool.push_back(new ConstantFieldRefInfo(this, reader_, tag));
      break;
    case CONSTANT_MethodRef:
      pool.push_back(new ConstantMethodRefInfo(this, reader_, tag));
      break;
    case CONSTANT_InterfaceMethodRef:
      pool.push_back(new ConstantInterfaceMethodRefInfo(this, reader_, tag));
      break;
    case CONSTANT_NameAndType:
      pool.push_back(new ConstantNameAndTypeInfo(this, reader_));
      break;
    case CONSTANT_MethodHandle:
      pool.push_back(new ConstantMethodHandleInfo(this, reader_));
      break;
    case CONSTANT_MethodType:
      pool.push_back(new ConstantMethodTypeInfo(this, reader_));
      break;
    case CONSTANT_Dynamic:
      pool.push_back(new ConstantDynamicInfo(this, reader_));
      break;
    case CONSTANT_InvokeDynamic:
      pool.push_back(new ConstantInvokeDynamicInfo(this, reader_));
      break;
    case CONSTANT_Module:
      pool.push_back(new ConstantModuleInfo(this, reader_));
      break;
    case CONSTANT_Package:
      pool.push_back(new ConstantPackageInfo(this, reader_));
      break;
    default:
      InvalidEntry ie(i, tag);
      throw ie;
    }
  }
}
ConstantPool::ConstantPool(std::vector<CpInfo *> pool_) { pool = pool_; }
u4 ConstantPool::Size() { return pool.size(); }
u4 ConstantPool::Length() {
  u4 length = 2;
  for (int i = 0; i < Size();) {
    CpInfo *info = pool[i];
    length += info->Length();
    i += info->Size();
  }
  return length;
}

} // namespace jvm