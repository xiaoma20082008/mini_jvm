//
// Created by machunxiao on 2020/4/6.
//

#include "runtime/ConstantPool.h"
#include "classfile/ClassReader.h"
#include <utility>

namespace jvm {

// region ConstantPool Info

CpInfo::CpInfo(u1 tag_) {
  cp = nullptr;
  tag = tag_;
}
CpInfo::CpInfo(ConstantPool* cp_, u1 tag_) {
  cp = cp_;
  tag = tag_;
}
u1 CpInfo::GetTag() { return tag; }
u4 CpInfo::Size() { return 1; }
CpInfo::~CpInfo() {}

// endregion ConstantPool Info

// region ConstantUtf8Info

ConstantUtf8Info::ConstantUtf8Info(ConstantPool* cp_, u1 tag_,
                                   std::string value_)
    : CpInfo(cp_, tag_) {
  cp = cp_;
  value = std::move(value_);
}

u4 ConstantUtf8Info::Length() { return 1 + value.size(); }
template <typename R, typename D>
R ConstantUtf8Info::Accept(ConstantInfoVisitor<R, D>* v, D* data) {
  return v->Visit(this, data);
}

// endregion ConstantUtf8Info

// region ConstantIntegerInfo

ConstantIntegerInfo::ConstantIntegerInfo(ConstantPool* cp_, u1 tag_, u4 bytes_)
    : CpInfo(cp_, tag_) {
  value = bytes_;
}

u4 ConstantIntegerInfo::Length() { return 5; }
template <typename R, typename D>
R ConstantIntegerInfo::Accept(ConstantInfoVisitor<R, D>* v, D* data) {
  return v->Visit(this, data);
};

// endregion ConstantIntegerInfo

// region ConstantFloatInfo

ConstantFloatInfo::ConstantFloatInfo(ConstantPool* cp_, u1 tag_, u4 bytes_)
    : CpInfo(cp_, tag_) {
  bytes = bytes_;
}
u4 ConstantFloatInfo::Length() { return 5; }
template <typename R, typename D>
R ConstantFloatInfo::Accept(ConstantInfoVisitor<R, D>* v, D* data) {
  return v->Visit(this, data);
};

// endregion ConstantFloatInfo

// region ConstantLongInfo

ConstantLongInfo::ConstantLongInfo(ConstantPool* cp_, u1 tag_, u4 high_bytes_,
                                   u4 low_bytes_)
    : CpInfo(cp_, tag_) {
  high_bytes = high_bytes_;
  low_bytes = low_bytes_;
}
u4 ConstantLongInfo::Size() { return 2; }
u4 ConstantLongInfo::Length() { return 9; }
template <typename R, typename D>
R ConstantLongInfo::Accept(ConstantInfoVisitor<R, D>* v, D* data) {
  return v->Visit(this, data);
};

// endregion ConstantLongInfo

// region ConstantDoubleInfo

ConstantDoubleInfo::ConstantDoubleInfo(ConstantPool* cp_, u1 tag_,
                                       u4 high_bytes_, u4 low_bytes_)
    : CpInfo(cp_, tag_) {
  high_bytes = high_bytes_;
  low_bytes = low_bytes_;
}
u4 ConstantDoubleInfo::Size() { return 2; }
u4 ConstantDoubleInfo::Length() { return 9; }
template <typename R, typename D>
R ConstantDoubleInfo::Accept(ConstantInfoVisitor<R, D>* v, D* data) {
  return v->Visit(this, data);
};

// endregion ConstantDoubleInfo

// region ConstantClassInfo

ConstantClassInfo::ConstantClassInfo(ConstantPool* cp_, u1 tag_, u2 name_index_)
    : CpInfo(cp_, tag_) {
  name_index = name_index_;
}

u4 ConstantClassInfo::Length() { return 3; }
template <typename R, typename D>
R ConstantClassInfo::Accept(ConstantInfoVisitor<R, D>* v, D* data) {
  return v->Visit(this, data);
}

std::string ConstantClassInfo::GetName() {
  return cp->GetUTF8Value(name_index);
}
// endregion ConstantClassInfo

// region ConstantStringInfo

ConstantStringInfo::ConstantStringInfo(ConstantPool* cp_, u1 tag_,
                                       u2 string_index_)
    : CpInfo(cp_, tag_) {
  string_index = string_index_;
}
u4 ConstantStringInfo::Length() { return 3; }
template <typename R, typename D>
R ConstantStringInfo::Accept(ConstantInfoVisitor<R, D>* v, D* data) {
  return v->Visit(this, data);
};

// endregion ConstantStringInfo

// region ConstantFieldRefInfo

ConstantFieldRefInfo::ConstantFieldRefInfo(ConstantPool* cp_, u1 tag_,
                                           u2 class_index_,
                                           u2 name_and_type_index_)
    : CpRef(cp_, tag_, class_index_, name_and_type_index_) {}
template <typename R, typename D>
R ConstantFieldRefInfo::Accept(ConstantInfoVisitor<R, D>* v, D* data) {
  return v->Visit(this, data);
};

// endregion ConstantFieldRefInfo

// region ConstantMethodRefInfo

ConstantMethodRefInfo::ConstantMethodRefInfo(ConstantPool* cp_, u1 tag_,
                                             u2 class_index_,
                                             u2 name_and_type_index_)
    : CpRef(cp_, tag_, class_index_, name_and_type_index_) {}
template <typename R, typename D>
R ConstantMethodRefInfo::Accept(ConstantInfoVisitor<R, D>* v, D* data) {
  return v->Visit(this, data);
};

// endregion ConstantMethodRefInfo

// region ConstantInterfaceMethodRefInfo

ConstantInterfaceMethodRefInfo::ConstantInterfaceMethodRefInfo(
    ConstantPool* cp_, u1 tag_, u2 class_index_, u2 name_and_type_index_)
    : CpRef(cp_, tag_, class_index_, name_and_type_index_) {}
template <typename R, typename D>
R ConstantInterfaceMethodRefInfo::Accept(ConstantInfoVisitor<R, D>* v,
                                         D* data) {
  return v->Visit(this, data);
};

// endregion ConstantInterfaceMethodRefInfo

// region ConstantNameAndTypeInfo

ConstantNameAndTypeInfo::ConstantNameAndTypeInfo(ConstantPool* cp_, u1 tag_,
                                                 u2 name_index_,
                                                 u2 descriptor_index_)
    : CpInfo(cp_, tag_) {
  name_index = name_index_;
  descriptor_index = descriptor_index_;
}
u4 ConstantNameAndTypeInfo::Length() { return 4; }
template <typename R, typename D>
R ConstantNameAndTypeInfo::Accept(ConstantInfoVisitor<R, D>* v, D* data) {
  return v->Visit(this, data);
};

// endregion ConstantNameAndTypeInfo

// region ConstantMethodHandleInfo

ConstantMethodHandleInfo::ConstantMethodHandleInfo(ConstantPool* cp_, u1 tag_,
                                                   u1 reference_kind_,
                                                   u2 reference_index_)
    : CpInfo(cp_, tag_) {
  reference_kind = reference_kind_;
  reference_index = reference_index_;
}
u4 ConstantMethodHandleInfo::Length() { return 4; }
template <typename R, typename D>
R ConstantMethodHandleInfo::Accept(ConstantInfoVisitor<R, D>* v, D* data) {
  return v->Visit(this, data);
};

// endregion ConstantMethodHandleInfo

// region ConstantMethodTypeInfo

ConstantMethodTypeInfo::ConstantMethodTypeInfo(ConstantPool* cp_, u1 tag_,
                                               u2 descriptor_index_)
    : CpInfo(cp_, tag_) {
  descriptor_index = descriptor_index_;
}
u4 ConstantMethodTypeInfo::Length() { return 3; }
template <typename R, typename D>
R ConstantMethodTypeInfo::Accept(ConstantInfoVisitor<R, D>* v, D* data) {
  return v->Visit(this, data);
};

// endregion ConstantMethodTypeInfo

// region ConstantDynamicInfo

ConstantDynamicInfo::ConstantDynamicInfo(ConstantPool* cp_, u1 tag_,
                                         u2 bootstrap_method_attr_index_,
                                         u2 name_and_type_index_)
    : CpInfo(cp_, tag_) {
  bootstrap_method_attr_index = bootstrap_method_attr_index_;
  name_and_type_index = name_and_type_index_;
}
u4 ConstantDynamicInfo::Length() { return 5; }
template <typename R, typename D>
R ConstantDynamicInfo::Accept(ConstantInfoVisitor<R, D>* v, D* data) {
  return v->Visit(this, data);
};

// endregion ConstantDynamicInfo

// region ConstantInvokeDynamicInfo

ConstantInvokeDynamicInfo::ConstantInvokeDynamicInfo(
    ConstantPool* cp_, u1 tag_, u2 bootstrap_method_attr_index_,
    u2 name_and_type_index_)
    : CpInfo(cp_, tag_) {
  bootstrap_method_attr_index = bootstrap_method_attr_index_;
  name_and_type_index = name_and_type_index_;
}
u4 ConstantInvokeDynamicInfo::Length() { return 5; }
template <typename R, typename D>
R ConstantInvokeDynamicInfo::Accept(ConstantInfoVisitor<R, D>* v, D* data) {
  return v->Visit(this, data);
};

// endregion ConstantInvokeDynamicInfo

// region ConstantModuleInfo

ConstantModuleInfo::ConstantModuleInfo(ConstantPool* cp_, u1 tag_,
                                       u2 name_index_)
    : CpInfo(cp_, tag_) {
  name_index = name_index_;
}
u4 ConstantModuleInfo::Length() { return 3; }
template <typename R, typename D>
R ConstantModuleInfo::Accept(ConstantInfoVisitor<R, D>* v, D* data) {
  return v->Visit(this, data);
};

// endregion ConstantModuleInfo

// region ConstantPackageInfo

ConstantPackageInfo::ConstantPackageInfo(ConstantPool* cp_, u1 tag_,
                                         u2 name_index_)
    : CpInfo(cp_, tag_) {
  name_index = name_index_;
}
u4 ConstantPackageInfo::Length() { return 3; }
template <typename R, typename D>
R ConstantPackageInfo::Accept(ConstantInfoVisitor<R, D>* v, D* data) {
  return v->Visit(this, data);
};

// endregion ConstantPackageInfo

// region CpRef

CpRef::CpRef(ConstantPool* cp_, u1 tag_, u2 class_index_,
             u2 name_and_type_index_)
    : CpInfo(cp_, tag_) {
  cp = cp_;
  tag = tag_;
  class_index = class_index_;
  name_and_type_index = name_and_type_index_;
}

u4 CpRef::Length() { return 5; }

// endregion CpRef

// region ConstantPool

// ConstantPool::ConstantPool(ClassReader* reader_) {
//  constant_pool_count = reader_->ReadUInt2();
//  pool = new CpInfo*[constant_pool_count];
//  for (u2 i = 1; i < constant_pool_count; ++i) {
//    u1 tag = reader_->ReadUInt1();
//    CpInfo* info = nullptr;
//    switch (tag) {
//    case CONSTANT_Utf8:
//      info = (new ConstantUtf8Info(this, tag, reader_->ReadUTF()));
//      break;
//    case CONSTANT_Integer:
//      info = (new ConstantIntegerInfo(this, tag, reader_->ReadInt4()));
//      break;
//    case CONSTANT_Float:
//      info = (new ConstantFloatInfo(this, tag, reader_->ReadInt4()));
//      break;
//    case CONSTANT_Long:
//      info = (new ConstantLongInfo(this, tag, reader_->ReadInt4(),
//                                   reader_->ReadInt4()));
//      i++;
//      break;
//    case CONSTANT_Double:
//      info = (new ConstantDoubleInfo(this, tag, reader_->ReadInt4(),
//                                     reader_->ReadInt4()));
//      i++;
//      break;
//    case CONSTANT_Class:
//      info = (new ConstantClassInfo(this, tag, reader_->ReadUInt2()));
//      break;
//    case CONSTANT_String:
//      info = (new ConstantStringInfo(this, tag, reader_->ReadUInt2()));
//      break;
//    case CONSTANT_FieldRef:
//      info = (new ConstantFieldRefInfo(this, tag, reader_->ReadUInt2(),
//                                       reader_->ReadUInt2()));
//      break;
//    case CONSTANT_MethodRef:
//      info = (new ConstantMethodRefInfo(this, tag, reader_->ReadUInt2(),
//                                        reader_->ReadUInt2()));
//      break;
//    case CONSTANT_InterfaceMethodRef:
//      info = (new ConstantInterfaceMethodRefInfo(
//          this, tag, reader_->ReadUInt2(), reader_->ReadUInt2()));
//      break;
//    case CONSTANT_NameAndType:
//      info = (new ConstantNameAndTypeInfo(this, tag, reader_->ReadUInt2(),
//                                          reader_->ReadUInt2()));
//      break;
//    case CONSTANT_MethodHandle:
//      info = (new ConstantMethodHandleInfo(this, tag, reader_->ReadUInt1(),
//                                           reader_->ReadUInt2()));
//      break;
//    case CONSTANT_MethodType:
//      info = (new ConstantMethodTypeInfo(this, tag, reader_->ReadUInt2()));
//      break;
//    case CONSTANT_Dynamic:
//      info = (new ConstantDynamicInfo(this, tag, reader_->ReadUInt2(),
//                                      reader_->ReadUInt2()));
//      break;
//    case CONSTANT_InvokeDynamic:
//      info = (new ConstantInvokeDynamicInfo(this, tag, reader_->ReadUInt2(),
//                                            reader_->ReadUInt2()));
//      break;
//    case CONSTANT_Module:
//      info = (new ConstantModuleInfo(this, tag, reader_->ReadUInt2()));
//      break;
//    case CONSTANT_Package:
//      info = (new ConstantPackageInfo(this, tag, reader_->ReadUInt2()));
//      break;
//    default:
//      throw InvalidEntry(i, tag);
//    }
//    pool[i] = info;
//  }
//}

u4 ConstantPool::Size() { return constant_pool_count; }
u4 ConstantPool::Length() {
  u4 length = 2;
  for (int i = 0; i < Size();) {
    CpInfo* info = pool[i];
    length += info->Length();
    i += info->Size();
  }
  return length;
}
u4 ConstantPool::GetUTF8Index(const std::string& value_) {
  for (int i = 0; i < constant_pool_count; ++i) {
    CpInfo* info = pool[i];
    if (info->GetTag() == CONSTANT_Utf8) {
      auto* utf8Info = dynamic_cast<ConstantUtf8Info*>(info);
      if (utf8Info != nullptr && utf8Info->value == value_) {
        return i;
      }
    }
  }
  throw EntryNotFound(value_);
}
std::string ConstantPool::GetUTF8Value(u4 index) {
  return GetUTF8Info(index)->value;
}
CpInfo* ConstantPool::Get(u4 index) {
  if (index < 0 || index >= constant_pool_count) {
    throw InvalidIndex(index);
  }
  CpInfo* cp = pool[index];
  if (cp == nullptr) {
    throw InvalidIndex(index);
  }
  return cp;
}
CpInfo* ConstantPool::Get(u4 index, u1 expected_type) {
  auto info = Get(index);
  if (info == nullptr || info->GetTag() != expected_type) {
    throw UnexpectedEntry(index, expected_type, info->GetTag());
  }
  return info;
}
ConstantUtf8Info* ConstantPool::GetUTF8Info(u4 index) {
  return dynamic_cast<ConstantUtf8Info*>(Get(index, CONSTANT_Utf8));
}
ConstantClassInfo* ConstantPool::GetClassInfo(u4 index) {
  return dynamic_cast<ConstantClassInfo*>(Get(index, CONSTANT_Class));
}
ConstantModuleInfo* ConstantPool::GetModuleInfo(u4 index) {
  return dynamic_cast<ConstantModuleInfo*>(Get(index, CONSTANT_Module));
}
ConstantNameAndTypeInfo* ConstantPool::GetNameAndTypeInfo(u4 index) {
  return dynamic_cast<ConstantNameAndTypeInfo*>(
      Get(index, CONSTANT_NameAndType));
}
ConstantPackageInfo* ConstantPool::GetPackageInfo(u4 index) {
  return dynamic_cast<ConstantPackageInfo*>(Get(index, CONSTANT_Package));
}

// endregion ConstantPool

} // namespace jvm