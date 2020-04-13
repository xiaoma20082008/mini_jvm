#ifndef MINI_JVM_ANNOTATION_H
#define MINI_JVM_ANNOTATION_H
#include "Common.h"
#include <vector>
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
  u1 tag = 0;
};
struct Primitive_element_value : public element_value {

  Primitive_element_value(ClassReader *reader_, u1 tag_);

public:
  u2 const_value_index = 0;
};
struct Enum_element_value : public element_value {
  Enum_element_value(ClassReader *reader_, u1 tag_);

public:
  u2 type_name_index = 0;
  u2 const_name_index = 0;
};
struct Class_element_value : public element_value {
  Class_element_value(ClassReader *reader_, u1 tag_);

public:
  u2 class_info_index = 0;
};
struct Annotation_element_value : public element_value {
  Annotation_element_value(ClassReader *reader_, u1 tag_);
  ~Annotation_element_value();

public:
  Annotation *annotation = nullptr;
};
struct Array_element_value : public element_value {
  Array_element_value(ClassReader *reader_, u1 tag_);
  ~Array_element_value();

public:
  u2 num_values = 0;
  element_value **values = nullptr;
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
  std::vector<element_value_pair *> element_value_pairs;
};

} // namespace jvm
#endif