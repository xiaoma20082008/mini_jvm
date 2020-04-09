#ifndef MINI_JVM_ANNOTATION_H
#define MINI_JVM_ANNOTATION_H
#include "Common.h"
namespace jvm {
class ClassReader;

struct Annotation;
struct element_value {
public:
  static element_value *Read(ClassReader *reader_);

public:
  element_value(u1 tag_);

public:
  u4 Length();
  u1 tag;
};
struct Primitive_element_value : public element_value {

  Primitive_element_value(ClassReader *reader_, u1 tag_);

public:
  u2 const_value_index;
};
struct Enum_element_value : public element_value {
  Enum_element_value(ClassReader *reader_, u1 tag_);

public:
  u2 type_name_index;
  u2 const_name_index;
};
struct Class_element_value : public element_value {
  Class_element_value(ClassReader *reader_, u1 tag_);

public:
  u2 class_info_index;
};
struct Annotation_element_value : public element_value {
  Annotation_element_value(ClassReader *reader_, u1 tag_);
  ~Annotation_element_value();

public:
  Annotation *annotation;
};
struct Array_element_value : public element_value {
  Array_element_value(ClassReader *reader_, u1 tag_);
  ~Array_element_value();

public:
  u2 num_values;
  element_value **values;
};

//

struct element_value_pair {
public:
  element_value_pair(ClassReader *reader_);
  ~element_value_pair();

public:
  u2 element_name_index;
  element_value *value;
};

//

struct Annotation {
public:
  Annotation(ClassReader *reader);
  ~Annotation();

public:
  u2 type_index;
  u2 num_element_value_pairs;
  element_value_pair **element_value_pairs;
};

} // namespace jvm
#endif