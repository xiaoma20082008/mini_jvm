//
// Created by machunxiao on 2020/4/7.
//
#include "Field.h"
#include "ClassReader.h"
#include "ClassWriter.h"

namespace jvm {

Field::Field(ClassReader *reader) {
  access_flags = reader->ReadUInt2();
  name_index = reader->ReadUInt2();
  descriptor_index = reader->ReadUInt2();
  attributes = new Attributes(reader);
}

Field::Field(u2 access_flags_, u2 name_index_, u2 descriptor_index_,
             Attributes *attributes_) {
  access_flags = access_flags_;
  name_index = name_index_;
  descriptor_index = descriptor_index_;
  attributes = attributes_;
}

Field::~Field() { delete attributes; }

} // namespace jvm