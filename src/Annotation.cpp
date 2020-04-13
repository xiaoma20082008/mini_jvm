#include "Annotation.h"
#include "ClassReader.h"
#include "ClassWriter.h"
#include "Exception.h"
namespace jvm {

element_value *element_value::Read(ClassReader *reader_) {
  u1 tag = reader_->ReadUInt1();
  switch (tag) {
  case 'B':
  case 'C':
  case 'D':
  case 'F':
  case 'I':
  case 'J':
  case 'S':
  case 'Z':
  case 's':
    return new Primitive_element_value(reader_, tag);

  case 'e':
    return new Enum_element_value(reader_, tag);

  case 'c':
    return new Class_element_value(reader_, tag);

  case '@':
    return new Annotation_element_value(reader_, tag);

  case '[':
    return new Array_element_value(reader_, tag);

  default:
    std::string s("unrecognized tag: ");
    s.append(std::to_string(tag));
    throw InvalidAnnotation(s);
  }
}

element_value::element_value(u1 tag_) { tag = tag_; }

Primitive_element_value::Primitive_element_value(jvm::ClassReader *reader_,
                                                 u1 tag_)
    : element_value(tag_) {
  const_value_index = reader_->ReadUInt2();
}

Enum_element_value::Enum_element_value(jvm::ClassReader *reader_, u1 tag_)
    : element_value(tag_) {
  type_name_index = reader_->ReadUInt2();
  const_name_index = reader_->ReadUInt2();
}

Class_element_value::Class_element_value(jvm::ClassReader *reader_, u1 tag_)
    : element_value(tag_) {
  class_info_index = reader_->ReadUInt2();
}

Annotation_element_value::Annotation_element_value(jvm::ClassReader *reader_,
                                                   u1 tag_)
    : element_value(tag_) {
  annotation = new Annotation(reader_);
}
Annotation_element_value::~Annotation_element_value() { delete annotation; }

Array_element_value::Array_element_value(jvm::ClassReader *reader_, u1 tag_)
    : element_value(tag_) {
  num_values = reader_->ReadUInt2();
  values = new element_value *[num_values];
  for (u2 i = 0; i < num_values; ++i) {
    values[i] = element_value::Read(reader_);
  }
}
Array_element_value::~Array_element_value() { delete[] values; }

element_value_pair::element_value_pair(jvm::ClassReader *reader_) {
  element_name_index = reader_->ReadUInt2();
  value = element_value::Read(reader_);
}
element_value_pair::~element_value_pair() { delete value; }

Annotation::Annotation(jvm::ClassReader *reader) {
  type_index = reader->ReadUInt2();
  num_element_value_pairs = reader->ReadUInt2();
  element_value_pairs.reserve(num_element_value_pairs);
  for (u2 i = 0; i < num_element_value_pairs; ++i) {
    element_value_pairs.push_back(new element_value_pair(reader));
  }
}
Annotation::~Annotation() {
  for (auto &ev : element_value_pairs) {
    delete ev;
  }
  element_value_pairs.clear();
}
} // namespace jvm