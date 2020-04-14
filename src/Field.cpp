//
// Created by machunxiao on 2020/4/7.
//
#include "Field.h"
#include "AccessFlags.h"
#include "ClassReader.h"
#include "ClassWriter.h"
#include "ConstantPool.h"
namespace jvm {

Field::Field(ClassReader *reader) {
  access_flags = new AccessFlags(reader->ReadUInt2());
  name_index = reader->ReadUInt2();
  descriptor_index = reader->ReadUInt2();
  attributes = new Attributes(reader);
}

Field::Field(u2 access_flags_, u2 name_index_, u2 descriptor_index_,
             Attributes *attributes_) {
  access_flags = new AccessFlags(access_flags_);
  name_index = name_index_;
  descriptor_index = descriptor_index_;
  attributes = attributes_;
}

Field::~Field() {
  delete attributes;
  delete access_flags;
}
std::string Field::GetName(ConstantPool *pool) {
  return pool->GetUTF8Value(name_index);
}
} // namespace jvm