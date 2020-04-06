//
// Created by machunxiao on 2020/4/6.
//

#ifndef MINI_JVM_CONSTANTPOOL_H
#define MINI_JVM_CONSTANTPOOL_H

#include "ClassReader.h"
#include "ClassVisitor.h"
#include "ClassWriter.h"
#include <exception>
#include <memory>
#include <vector>
namespace jvm {

// region exception

class ConstantPoolException : public std::exception {

public:
  ConstantPoolException(u4 index_) : index(index_) {}
  u4 index;
};

class InvalidEntry : public ConstantPoolException {

public:
  InvalidEntry(u4 index_, u1 tag_) : ConstantPoolException(index_) {
    tag = tag_;
  }
  u1 tag;
};

// endregion exception

// region ConstantInfo

class ConstantPool;

struct CpInfo {
  u1 tag;
  ConstantPool *cp;
  CpInfo() { cp = nullptr; }
  CpInfo(ConstantPool *cp_) { cp = cp_; }
  virtual ~CpInfo() = default;
  virtual u4 Size() { return 1; };
  virtual u4 Length() = 0;
  virtual u1 GetTag() = 0;
  template <typename R, typename D> R Accept(Visitor<R, D> *v, D *data) {
    return v->Visit(this, data);
  };
};
struct ConstantClassInfo : public CpInfo {
  u2 name_index;
  ConstantClassInfo(ConstantPool *cp_, ClassReader *reader) : CpInfo(cp_) {
    name_index = reader->ReadUInt2();
  }
  ConstantClassInfo(ConstantPool *cp_, u2 name_index_) : CpInfo(cp_) {
    name_index = name_index_;
  }
  u4 Length() override { return 3; }
  u1 GetTag() override { return CONSTANT_Class; }
  template <typename R, typename D> R Accept(Visitor<R, D> *v, D *data) {
    return v->Visit(this, data);
  };
};
struct CpRef : public CpInfo {
  u2 class_index;
  u2 name_and_type_index;

  CpRef(ConstantPool *cp_, ClassReader *reader, u1 tag_) : CpInfo(cp_) {
    tag = tag_;
    class_index = reader->ReadUInt2();
    name_and_type_index = reader->ReadUInt2();
  }
  CpRef(ConstantPool *cp_, u1 tag_, u2 class_index_, u2 name_and_type_index_) {
    cp = cp_;
    tag = tag_;
    class_index = class_index_;
    name_and_type_index = name_and_type_index_;
  }

  u4 Length() override { return 5; }
  template <typename R, typename D> R Accept(Visitor<R, D> *v, D *data) {
    return v->Visit(this, data);
  };
};
struct ConstantFieldRefInfo : public CpRef {
  ConstantFieldRefInfo(ConstantPool *cp_, ClassReader *reader_, u1 tag_)
      : CpRef(cp_, reader_, tag_) {}
  ConstantFieldRefInfo(ConstantPool *cp_, u1 tag_, u2 class_index_,
                       u2 name_and_type_index_)
      : CpRef(cp_, tag_, class_index_, name_and_type_index_) {}
  u4 Length() override { return 5; }
  u1 GetTag() override { return CONSTANT_FieldRef; }
  template <typename R, typename D> R Accept(Visitor<R, D> *v, D *data) {
    return v->Visit(this, data);
  };
};
struct ConstantMethodRefInfo : public CpRef {
  ConstantMethodRefInfo(ConstantPool *cp_, ClassReader *reader_, u1 tag_)
      : CpRef(cp_, reader_, tag_) {}
  ConstantMethodRefInfo(ConstantPool *cp_, u1 tag_, u2 class_index_,
                        u2 name_and_type_index_)
      : CpRef(cp_, tag_, class_index_, name_and_type_index_) {}
  u4 Length() override { return 5; }
  u1 GetTag() override { return CONSTANT_MethodRef; }
  template <typename R, typename D> R Accept(Visitor<R, D> *v, D *data) {
    return v->Visit(this, data);
  };
};
struct ConstantInterfaceMethodRefInfo : public CpRef {
  ConstantInterfaceMethodRefInfo(ConstantPool *cp_, ClassReader *reader_,
                                 u1 tag_)
      : CpRef(cp_, reader_, tag_) {}
  ConstantInterfaceMethodRefInfo(ConstantPool *cp_, u1 tag_, u2 class_index_,
                                 u2 name_and_type_index_)
      : CpRef(cp_, tag_, class_index_, name_and_type_index_) {}
  u4 Length() override { return 5; }
  u1 GetTag() override { return CONSTANT_InterfaceMethodRef; }
  template <typename R, typename D> R Accept(Visitor<R, D> *v, D *data) {
    return v->Visit(this, data);
  };
};
struct ConstantStringInfo : public CpInfo {
  u2 string_index;
  ConstantStringInfo(ConstantPool *cp_, ClassReader *reader) : CpInfo(cp_) {
    string_index = reader->ReadUInt2();
  }
  ConstantStringInfo(ConstantPool *cp_, u2 string_index_) : CpInfo(cp_) {
    string_index = string_index_;
  }
  u4 Length() override { return 3; }
  u1 GetTag() override { return CONSTANT_String; }
  template <typename R, typename D> R Accept(Visitor<R, D> *v, D *data) {
    return v->Visit(this, data);
  };
};
struct ConstantIntegerInfo : public CpInfo {
  u4 bytes;
  ConstantIntegerInfo(ConstantPool *cp_, ClassReader *reader) : CpInfo(cp_) {
    bytes = reader->ReadUInt4();
  }
  ConstantIntegerInfo(ConstantPool *cp_, u4 bytes_) : CpInfo(cp_) {
    bytes = bytes_;
  }
  u4 Length() override { return 5; }
  u1 GetTag() override { return CONSTANT_Integer; }
  template <typename R, typename D> R Accept(Visitor<R, D> *v, D *data) {
    return v->Visit(this, data);
  };
};
struct ConstantFloatInfo : public CpInfo {
  u4 bytes;
  ConstantFloatInfo(ConstantPool *cp_, ClassReader *reader) : CpInfo(cp_) {
    bytes = reader->ReadUInt4();
  }
  ConstantFloatInfo(ConstantPool *cp_, u4 bytes_) : CpInfo(cp_) {
    bytes = bytes_;
  }
  u4 Length() override { return 5; }
  u1 GetTag() override { return CONSTANT_Float; }
  template <typename R, typename D> R Accept(Visitor<R, D> *v, D *data) {
    return v->Visit(this, data);
  };
};
struct ConstantLongInfo : public CpInfo {
  u4 high_bytes;
  u4 low_bytes;
  ConstantLongInfo(ConstantPool *cp_, ClassReader *reader) : CpInfo(cp_) {
    high_bytes = reader->ReadUInt4();
    low_bytes = reader->ReadUInt4();
  }
  ConstantLongInfo(ConstantPool *cp_, u4 high_bytes_, u4 low_bytes_)
      : CpInfo(cp_) {
    high_bytes = high_bytes_;
    low_bytes = low_bytes_;
  }
  u4 Size() override { return 2; }
  u4 Length() override { return 9; }
  u1 GetTag() override { return CONSTANT_Long; }
  template <typename R, typename D> R Accept(Visitor<R, D> *v, D *data) {
    return v->Visit(this, data);
  };
};
struct ConstantDoubleInfo : public CpInfo {
  u4 high_bytes;
  u4 low_bytes;
  ConstantDoubleInfo(ConstantPool *cp_, ClassReader *reader) : CpInfo(cp_) {
    high_bytes = reader->ReadUInt4();
    low_bytes = reader->ReadUInt4();
  }
  ConstantDoubleInfo(ConstantPool *cp_, u4 high_bytes_, u4 low_bytes_)
      : CpInfo(cp_) {
    high_bytes = high_bytes_;
    low_bytes = low_bytes_;
  }
  u4 Size() override { return 2; }
  u4 Length() override { return 9; }
  u1 GetTag() override { return CONSTANT_Double; }
  template <typename R, typename D> R Accept(Visitor<R, D> *v, D *data) {
    return v->Visit(this, data);
  };
};
struct ConstantNameAndTypeInfo : public CpInfo {
  u2 name_index;
  u2 descriptor_index;
  ConstantNameAndTypeInfo(ConstantPool *cp_, ClassReader *reader)
      : CpInfo(cp_) {
    name_index = reader->ReadUInt2();
    descriptor_index = reader->ReadUInt2();
  }
  ConstantNameAndTypeInfo(ConstantPool *cp_, u2 name_index_,
                          u2 descriptor_index_)
      : CpInfo(cp_) {
    name_index = name_index_;
    descriptor_index = descriptor_index_;
  }
  u4 Length() override { return 4; }
  u1 GetTag() override { return CONSTANT_NameAndType; }
  template <typename R, typename D> R Accept(Visitor<R, D> *v, D *data) {
    return v->Visit(this, data);
  };
};
struct ConstantUtf8Info : public CpInfo {
  //  u2 length;
  //  u1 bytes[length];
  ConstantUtf8Info(ConstantPool *cp_, ClassReader *reader) : CpInfo(cp_) {
    u2 size = reader->ReadUInt2();
    for (int i = 0; i < size; ++i) {
      bytes.push_back(reader->ReadUInt1());
    }
  }
  ConstantUtf8Info(ConstantPool *cp_, std::vector<u1> &bytes_) : CpInfo(cp_) {
    bytes = bytes_;
  }
  std::vector<u1> bytes;
  u4 Length() override { return 1 + bytes.size(); }
  u1 GetTag() override { return CONSTANT_Utf8; }
  template <typename R, typename D> R Accept(Visitor<R, D> *v, D *data) {
    return v->Visit(this, data);
  };
};
struct ConstantMethodHandleInfo : public CpInfo {
  u1 reference_kind;
  u2 reference_index;
  ConstantMethodHandleInfo(ConstantPool *cp_, ClassReader *reader)
      : CpInfo(cp_) {
    reference_kind = reader->ReadUInt1();
    reference_index = reader->ReadUInt2();
  }
  ConstantMethodHandleInfo(ConstantPool *cp_, u1 reference_kind_,
                           u2 reference_index_)
      : CpInfo(cp_) {
    reference_kind = reference_kind_;
    reference_index = reference_index_;
  }
  u4 Length() override { return 4; }
  u1 GetTag() override { return CONSTANT_MethodHandle; }
  template <typename R, typename D> R Accept(Visitor<R, D> *v, D *data) {
    return v->Visit(this, data);
  };
};
struct ConstantMethodTypeInfo : public CpInfo {
  u2 descriptor_index;
  ConstantMethodTypeInfo(ConstantPool *cp_, ClassReader *reader) : CpInfo(cp_) {
    descriptor_index = reader->ReadUInt2();
  }
  ConstantMethodTypeInfo(ConstantPool *cp_, u2 descriptor_index_)
      : CpInfo(cp_) {
    descriptor_index = descriptor_index_;
  }
  u4 Length() override { return 3; }
  u1 GetTag() override { return CONSTANT_MethodType; }
  template <typename R, typename D> R Accept(Visitor<R, D> *v, D *data) {
    return v->Visit(this, data);
  };
};
struct ConstantDynamicInfo : public CpInfo {
  u2 bootstrap_method_attr_index;
  u2 name_and_type_index;
  ConstantDynamicInfo(ConstantPool *cp_, ClassReader *reader) : CpInfo(cp_) {
    bootstrap_method_attr_index = reader->ReadUInt2();
    name_and_type_index = reader->ReadUInt2();
  }
  ConstantDynamicInfo(ConstantPool *cp_, u2 bootstrap_method_attr_index_,
                      u2 name_and_type_index_)
      : CpInfo(cp_) {
    bootstrap_method_attr_index = bootstrap_method_attr_index_;
    name_and_type_index = name_and_type_index_;
  }
  u4 Length() override { return 5; }
  u1 GetTag() override { return CONSTANT_Dynamic; }
  template <typename R, typename D> R Accept(Visitor<R, D> *v, D *data) {
    return v->Visit(this, data);
  };
};
struct ConstantInvokeDynamicInfo : public CpInfo {
  u2 bootstrap_method_attr_index;
  u2 name_and_type_index;
  ConstantInvokeDynamicInfo(ConstantPool *cp_, ClassReader *reader)
      : CpInfo(cp_) {
    bootstrap_method_attr_index = reader->ReadUInt2();
    name_and_type_index = reader->ReadUInt2();
  }
  ConstantInvokeDynamicInfo(ConstantPool *cp_, u2 bootstrap_method_attr_index_,
                            u2 name_and_type_index_)
      : CpInfo(cp_) {
    bootstrap_method_attr_index = bootstrap_method_attr_index_;
    name_and_type_index = name_and_type_index_;
  }
  u4 Length() override { return 5; }
  u1 GetTag() override { return CONSTANT_InvokeDynamic; }
  template <typename R, typename D> R Accept(Visitor<R, D> *v, D *data) {
    return v->Visit(this, data);
  };
};
struct ConstantModuleInfo : public CpInfo {
  u2 name_index;
  ConstantModuleInfo(ConstantPool *cp_, ClassReader *reader) : CpInfo(cp_) {
    name_index = reader->ReadUInt2();
  }
  ConstantModuleInfo(ConstantPool *cp_, u2 name_index_) : CpInfo(cp_) {
    name_index = name_index_;
  }
  u4 Length() override { return 3; }
  u1 GetTag() override { return CONSTANT_Module; }
  template <typename R, typename D> R Accept(Visitor<R, D> *v, D *data) {
    return v->Visit(this, data);
  };
};
struct ConstantPackageInfo : public CpInfo {
  u2 name_index;
  ConstantPackageInfo(ConstantPool *cp_, ClassReader *reader) : CpInfo(cp_) {
    name_index = reader->ReadUInt2();
  }
  ConstantPackageInfo(ConstantPool *cp_, u2 name_index_) : CpInfo(cp_) {
    name_index = name_index_;
  }
  u4 Length() override { return 3; }
  u1 GetTag() override { return CONSTANT_Package; }
  template <typename R, typename D> R Accept(Visitor<R, D> *v, D *data) {
    return v->Visit(this, data);
  };
};

struct AttributeInfo {
  u2 attribute_name_index;
  //  u4 attribute_length;
  //  u1 info[attribute_length];
  std::vector<u1> info;
};
struct FieldInfo {
  u2 access_flags;
  u2 name_index;
  u2 descriptor_index;
  //  u2             attributes_count;
  //  AttributeInfo attributes[attributes_count];
  std::vector<AttributeInfo> attributes;
};
struct MethodInfo {
  u2 access_flags;
  u2 name_index;
  u2 descriptor_index;
  //  u2             attributes_count;
  //  attribute_info attributes[attributes_count];
  std::vector<AttributeInfo> attributes;
};

class ConstantPool {
public:
  ConstantPool(ClassReader *reader_);
  ConstantPool(std::vector<CpInfo *> pool_);
  u4 Size();
  u4 Length();

private:
  std::vector<CpInfo *> pool;
};

// endregion ConstantInfo

} // namespace jvm
#endif // MINI_JVM_CONSTANTPOOL_H
