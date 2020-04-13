//
// Created by machunxiao on 2020/4/6.
//
#include "Attribute.h"
#include "ClassReader.h"
#include "ConstantPool.h"
#include "Exception.h"
#include <ClassFile.h>
namespace jvm {

// region attribute

Attribute::Attribute(u2 index_, u4 length_) {
  attribute_name_index = index_;
  attribute_length = length_;
}
u4 Attribute::Length() { return attribute_length + 6; }
Attribute *Attribute::Read(ClassReader *reader) {
  return reader->ReadAttribute();
}
std::string Attribute::GetName(ConstantPool *pool) {
  return pool->GetUTF8Value(attribute_name_index);
}
// region jvm attribute

ConstantValue_attribute::ConstantValue_attribute(ClassReader *reader_,
                                                 u2 index_, u4 length_)
    : Attribute(index_, length_) {
  constant_value_index = reader_->ReadUInt2();
}

template <typename R, typename D>
R ConstantValue_attribute::Accept(AttributeVisitor<R, D> *v, D *data) {
  return v->Visit(this, data);
}

Code_attribute::exception_data::exception_data(ClassReader *reader_) {
  start_pc = reader_->ReadUInt2();
  end_pc = reader_->ReadUInt2();
  handler_pc = reader_->ReadUInt2();
  catch_type = reader_->ReadUInt2();
}

Code_attribute::Code_attribute(ClassReader *reader_, u2 index_, u4 length_)
    : Attribute(index_, length_) {
  max_stack = reader_->ReadUInt2();
  max_locals = reader_->ReadUInt2();
  code_length = reader_->ReadInt4();
  code = new u1[code_length];
  reader_->ReadFully(code, 0, code_length);
  exception_table_length = reader_->ReadUInt2();
  exception_tables = new exception_data *[exception_table_length];
  for (int i = 0; i < exception_table_length; i++) {
    exception_tables[i] = new exception_data(reader_);
  }
  attributes = new Attributes(reader_);
}

template <typename R, typename D>
R Code_attribute::Accept(AttributeVisitor<R, D> *v, D *data) {
  return v->Visit(this, data);
}

StackMapTable_attribute::verification_type_info *
StackMapTable_attribute::verification_type_info::Read(
    jvm::ClassReader *reader_) {
  u1 tag = reader_->ReadUInt1();
  switch (tag) {
  case ITEM_Top:
  case ITEM_Integer:
  case ITEM_Float:
  case ITEM_Double:
  case ITEM_Long:
  case ITEM_Null:
  case ITEM_UninitializedThis:
    return new StackMapTable_attribute::verification_type_info(tag);
  case ITEM_Object:
    return new StackMapTable_attribute::Object_variable_info(reader_);
  case ITEM_Uninitialized:
    return new StackMapTable_attribute::Uninitialized_variable_info(reader_);
  }
  throw IllegalArgumentException("unrecognized verification_type_info tag");
}

StackMapTable_attribute::verification_type_info::verification_type_info(
    u1 tag_) {
  tag = tag_;
}
u2 StackMapTable_attribute::verification_type_info::Length() { return 1; }

StackMapTable_attribute::Object_variable_info::Object_variable_info(
    jvm::ClassReader *reader_)
    : StackMapTable_attribute::verification_type_info(ITEM_Object) {
  cpool_index = reader_->ReadUInt2();
}

u2 StackMapTable_attribute::Object_variable_info::Length() {
  return super::Length() + 2;
}

StackMapTable_attribute::Uninitialized_variable_info::
    Uninitialized_variable_info(jvm::ClassReader *reader_)
    : StackMapTable_attribute::verification_type_info(ITEM_Uninitialized) {
  offset = reader_->ReadUInt2();
}

u2 StackMapTable_attribute::Uninitialized_variable_info::Length() {
  return super::Length() + 2;
}

// region frame

StackMapTable_attribute::same_frame::same_frame(u1 frame_type_)
    : stack_map_frame(frame_type_) {}
u1 StackMapTable_attribute::same_frame::Length() { return 1; }
u2 StackMapTable_attribute::same_frame::GetOffsetDelta() { return frame_type; }

StackMapTable_attribute::same_locals_1_stack_item_frame::
    same_locals_1_stack_item_frame(u1 frame_type_, ClassReader *reader_)
    : stack_map_frame(frame_type_) {
  stack = new verification_type_info *[1];
  stack[0] = StackMapTable_attribute::verification_type_info::Read(reader_);
}
u1 StackMapTable_attribute::same_locals_1_stack_item_frame::Length() {
  return 0;
}
u2 StackMapTable_attribute::same_locals_1_stack_item_frame::GetOffsetDelta() {
  return frame_type - 64;
}
StackMapTable_attribute::same_locals_1_stack_item_frame::
    same_locals_1_stack_item_frame::~same_locals_1_stack_item_frame() {
  delete[] stack;
}

StackMapTable_attribute::same_locals_1_stack_item_frame_extended::
    same_locals_1_stack_item_frame_extended(u1 frame_type_,
                                            ClassReader *reader_)
    : stack_map_frame(frame_type_) {
  stack = new verification_type_info *[1];
  stack[0] = StackMapTable_attribute::verification_type_info::Read(reader_);
}
u1 StackMapTable_attribute::same_locals_1_stack_item_frame_extended::Length() {
  return 0;
}
u2 StackMapTable_attribute::same_locals_1_stack_item_frame_extended::
    GetOffsetDelta() {
  return 0;
}
StackMapTable_attribute::same_locals_1_stack_item_frame_extended::
    same_locals_1_stack_item_frame_extended::
        ~same_locals_1_stack_item_frame_extended() {
  delete[] stack;
}

StackMapTable_attribute::chop_frame::chop_frame(u1 frame_type_,
                                                ClassReader *reader_)
    : stack_map_frame(frame_type_) {
  offset_delta = reader_->ReadUInt2();
}
u1 StackMapTable_attribute::chop_frame::Length() { return 0; }
u2 StackMapTable_attribute::chop_frame::GetOffsetDelta() { return 0; }

StackMapTable_attribute::same_frame_extended::same_frame_extended(
    u1 frame_type_, ClassReader *reader_)
    : stack_map_frame(frame_type_) {
  offset_delta = reader_->ReadUInt2();
}
u1 StackMapTable_attribute::same_frame_extended::Length() { return 0; }
u2 StackMapTable_attribute::same_frame_extended::GetOffsetDelta() { return 0; }

StackMapTable_attribute::append_frame::append_frame(u1 frame_type_,
                                                    ClassReader *reader_)
    : stack_map_frame(frame_type_) {
  offset_delta = reader_->ReadUInt2();
  u1 count = frame_type - 251;
  stack = new verification_type_info *[count];
  for (u1 i = 0; i < count; ++i) {
    stack[i] = verification_type_info::Read(reader_);
  }
}
u1 StackMapTable_attribute::append_frame::Length() { return 0; }
u2 StackMapTable_attribute::append_frame::GetOffsetDelta() { return 0; }

StackMapTable_attribute::full_frame::full_frame(u1 frame_type_,
                                                ClassReader *reader_)
    : stack_map_frame(frame_type_) {
  offset_delta = reader_->ReadUInt2();
  number_of_locals = reader_->ReadUInt2();
  locals = new verification_type_info *[number_of_locals];
  for (u2 i = 0; i < number_of_locals; i++) {
    locals[i] = verification_type_info::Read(reader_);
  }
  number_of_stack_items = reader_->ReadUInt2();
  stack = new verification_type_info *[number_of_stack_items];
  for (u2 i = 0; i < number_of_stack_items; i++) {
    stack[i] = verification_type_info::Read(reader_);
  }
}
u1 StackMapTable_attribute::full_frame::Length() { return 0; }
u2 StackMapTable_attribute::full_frame::GetOffsetDelta() { return 0; }

// endregion frame

StackMapTable_attribute::stack_map_frame *
StackMapTable_attribute::Read(ClassReader *reader_) {
  u1 frame_type = reader_->ReadUInt1();
  if (frame_type <= 63)
    return new same_frame(frame_type);
  else if (frame_type <= 127)
    return new same_locals_1_stack_item_frame(frame_type, reader_);
  else if (frame_type <= 246) {
    std::string s;
    s.append("unknown frame_type ");
    s.append(std::to_string(frame_type));
    throw std::invalid_argument(s);
  } else if (frame_type == 247)
    return new same_locals_1_stack_item_frame_extended(frame_type, reader_);
  else if (frame_type <= 250)
    return new chop_frame(frame_type, reader_);
  else if (frame_type == 251)
    return new same_frame_extended(frame_type, reader_);
  else if (frame_type <= 254)
    return new append_frame(frame_type, reader_);
  else
    return new full_frame(frame_type, reader_);
}

StackMapTable_attribute::stack_map_frame::stack_map_frame(u1 frame_type_) {
  frame_type = frame_type_;
}

u1 StackMapTable_attribute::stack_map_frame::Length() { return 1; }

StackMapTable_attribute::StackMapTable_attribute(ClassReader *reader_,
                                                 u2 index_, u4 length_)
    : Attribute(index_, length_) {
  number_of_entries = reader_->ReadUInt2();
  entries = new stack_map_frame *[number_of_entries];
  for (u2 i = 0; i < number_of_entries; ++i) {
    entries[i] = Read(reader_);
  }
}

StackMapTable_attribute::~StackMapTable_attribute() { delete[] entries; }

template <typename R, typename D>
R StackMapTable_attribute::Accept(AttributeVisitor<R, D> *v, D *data) {
  return v->Visit(this, data);
}

BootstrapMethods_attribute::BootStrapMethod::BootStrapMethod(
    ClassReader *reader_) {
  bootstrap_method_ref = reader_->ReadUInt2();
  num_bootstrap_arguments = reader_->ReadUInt2();
  bootstrap_arguments = new u2[num_bootstrap_arguments];
  for (u2 i = 0; i < num_bootstrap_arguments; ++i) {
    bootstrap_arguments[i] = reader_->ReadUInt2();
  }
}

BootstrapMethods_attribute::BootstrapMethods_attribute(ClassReader *reader_,
                                                       u2 index_, u4 length_)
    : Attribute(index_, length_) {
  num_bootstrap_methods = reader_->ReadUInt2();
  bootstrap_methods = new BootStrapMethod *[num_bootstrap_methods];
  for (u2 i = 0; i < num_bootstrap_methods; ++i) {
    bootstrap_methods[i] = new BootStrapMethod(reader_);
  }
}

template <typename R, typename D>
R BootstrapMethods_attribute::Accept(AttributeVisitor<R, D> *v, D *data) {
  return v->Visit(this, data);
}

NestHost_attribute::NestHost_attribute(ClassReader *reader_, u2 index_,
                                       u4 length_)
    : Attribute(index_, length_) {
  top_index = reader_->ReadUInt2();
}

template <typename R, typename P>
R NestHost_attribute::Accept(AttributeVisitor<R, P> *v, P *data) {
  return v->Visit(this, data);
}

NestMembers_attribute::NestMembers_attribute(ClassReader *reader_, u2 index_,
                                             u4 length_)
    : Attribute(index_, length_) {
  member_count = reader_->ReadUInt2();
  members_indexes = new u2[member_count];
  for (u2 i = 0; i < member_count; ++i) {
    members_indexes[i] = reader_->ReadUInt2();
  }
}

template <typename R, typename P>
R NestMembers_attribute::Accept(AttributeVisitor<R, P> *v, P *data) {
  return v->Visit(this, data);
}

// endregion jvm attribute

// region java se attribute

EnclosingMethod_attribute::EnclosingMethod_attribute(ClassReader *reader_,
                                                     u2 index_, u4 length_)
    : Attribute(index_, length_) {
  class_index = reader_->ReadUInt2();
  method_index = reader_->ReadUInt2();
}

template <typename R, typename D>
R EnclosingMethod_attribute::Accept(AttributeVisitor<R, D> *v, D *data) {
  return v->Visit(this, data);
}

Exceptions_attribute::Exceptions_attribute(ClassReader *reader_, u2 index_,
                                           u4 length_)
    : Attribute(index_, length_) {
  number_of_exceptions = reader_->ReadUInt2();
  exception_index_table = new u2[number_of_exceptions];
  for (u2 i = 0; i < number_of_exceptions; ++i) {
    exception_index_table[i] = reader_->ReadUInt2();
  }
}

Exceptions_attribute::~Exceptions_attribute() { delete exception_index_table; }

template <typename R, typename D>
R Exceptions_attribute::Accept(AttributeVisitor<R, D> *v, D *data) {
  return v->Visit(this, data);
}

InnerClasses_attribute::Info::Info(ClassReader *reader_) {
  inner_class_info_index = reader_->ReadUInt2();
  outer_class_info_index = reader_->ReadUInt2();
  inner_name_index = reader_->ReadUInt2();
  inner_class_access_flags = reader_->ReadUInt2();
}

InnerClasses_attribute::InnerClasses_attribute(ClassReader *reader_, u2 index_,
                                               u4 length_)
    : Attribute(index_, length_) {
  number_of_classes = reader_->ReadUInt2();
  classes = new Info *[number_of_classes];
  for (u2 i = 0; i < number_of_classes; ++i) {
    classes[i] = new Info(reader_);
  }
}

InnerClasses_attribute::~InnerClasses_attribute() { delete[] classes; }

template <typename R, typename D>
R InnerClasses_attribute::Accept(AttributeVisitor<R, D> *v, D *data) {
  return v->Visit(this, data);
}

LineNumberTable_attribute::LineNumberInfo::LineNumberInfo(
    ClassReader *reader_) {
  start_pc = reader_->ReadUInt2();
  line_number = reader_->ReadUInt2();
}

LineNumberTable_attribute::LineNumberTable_attribute(ClassReader *reader_,
                                                     u2 index_, u4 length_)
    : Attribute(index_, length_) {
  line_number_table_length = reader_->ReadUInt2();
  line_number_table = new LineNumberInfo *[line_number_table_length];
  for (u2 i = 0; i < line_number_table_length; ++i) {
    line_number_table[i] = new LineNumberInfo(reader_);
  }
}

LineNumberTable_attribute::~LineNumberTable_attribute() {
  delete[] line_number_table;
}

template <typename R, typename D>
R LineNumberTable_attribute::Accept(AttributeVisitor<R, D> *v, D *data) {
  return v->Visit(this, data);
}

LocalVariableTable_attribute::Entry::Entry(ClassReader *reader_) {
  start_pc = reader_->ReadUInt2();
  length = reader_->ReadUInt2();
  name_index = reader_->ReadUInt2();
  descriptor_index = reader_->ReadUInt2();
  index = reader_->ReadUInt2();
}

LocalVariableTable_attribute::LocalVariableTable_attribute(
    jvm::ClassReader *reader_, u2 index_, u4 length_)
    : Attribute(index_, length_) {
  local_variable_table_length = reader_->ReadUInt2();
  local_variable_table = new Entry *[local_variable_table_length];
  for (u2 i = 0; i < local_variable_table_length; ++i) {
    local_variable_table[i] = new Entry(reader_);
  }
}

LocalVariableTable_attribute::~LocalVariableTable_attribute() {
  delete[] local_variable_table;
}

template <typename R, typename D>
R LocalVariableTable_attribute::Accept(AttributeVisitor<R, D> *v, D *data) {
  return v->Visit(this, data);
}

LocalVariableTypeTable_attribute::Entry::Entry(ClassReader *reader_) {
  start_pc = reader_->ReadUInt2();
  length = reader_->ReadUInt2();
  name_index = reader_->ReadUInt2();
  signature_index = reader_->ReadUInt2();
  index = reader_->ReadUInt2();
}

LocalVariableTypeTable_attribute::LocalVariableTypeTable_attribute(
    jvm::ClassReader *reader_, u2 index_, u4 length_)
    : Attribute(index_, length_) {
  local_variable_type_table_length = reader_->ReadUInt2();
  local_variable_type_table = new Entry *[local_variable_type_table_length];
  for (u2 i = 0; i < local_variable_type_table_length; ++i) {
    local_variable_type_table[i] = new Entry(reader_);
  }
}

LocalVariableTypeTable_attribute::~LocalVariableTypeTable_attribute() {
  delete[] local_variable_type_table;
}

template <typename R, typename D>
R LocalVariableTypeTable_attribute::Accept(AttributeVisitor<R, D> *v, D *data) {
  return v->Visit(this, data);
}

Signature_attribute::Signature_attribute(jvm::ClassReader *reader_, u2 index_,
                                         u4 length_)
    : Attribute(index_, length_) {
  signature_index = reader_->ReadUInt2();
}

template <typename R, typename D>
R Signature_attribute::Accept(AttributeVisitor<R, D> *v, D *data) {
  return v->Visit(this, data);
}

SourceFile_attribute::SourceFile_attribute(jvm::ClassReader *reader_, u2 index_,
                                           u4 length_)
    : Attribute(index_, length_) {
  sourcefile_index = reader_->ReadUInt2();
}

std::string SourceFile_attribute::GetSourceFile(ConstantPool *pool) {
  return pool->GetUTF8Value(sourcefile_index);
}

template <typename R, typename D>
R SourceFile_attribute::Accept(AttributeVisitor<R, D> *v, D *data) {
  return v->Visit(this, data);
}

Synthetic_attribute::Synthetic_attribute(jvm::ClassReader *reader_, u2 index_,
                                         u4 length_)
    : Attribute(index_, length_) {}

template <typename R, typename D>
R Synthetic_attribute::Accept(AttributeVisitor<R, D> *v, D *data) {
  return v->Visit(this, data);
}

// endregion java se attribute

// region tools attribute

Deprecated_attribute::Deprecated_attribute(jvm::ClassReader *reader_, u2 index_,
                                           u4 length_)
    : Attribute(index_, length_) {}

template <typename R, typename D>
R Deprecated_attribute::Accept(AttributeVisitor<R, D> *v, D *data) {
  return v->Visit(this, data);
}

SourceDebugExtension_attribute::SourceDebugExtension_attribute(
    jvm::ClassReader *reader_, u2 index_, u4 length_)
    : Attribute(index_, length_) {
  attribute_length = reader_->ReadInt4();
  debug_extension = new u1[attribute_length];
  reader_->ReadFully(debug_extension, 0, attribute_length);
}

std::string SourceDebugExtension_attribute::GetValue() {
  std::string s(reinterpret_cast<const char *>(debug_extension),
                attribute_length);
  return s;
}

AnnotationDefault_attribute::AnnotationDefault_attribute(
    jvm::ClassReader *reader_, u2 index_, u4 length_)
    : Attribute(index_, length_) {
  value = element_value::Read(reader_);
}

template <typename R, typename D>
R AnnotationDefault_attribute::Accept(AttributeVisitor<R, D> *v, D *data) {
  return v->Visit(this, data);
}

RuntimeAnnotations_attribute::RuntimeAnnotations_attribute(
    jvm::ClassReader *reader_, u2 index_, u4 length_)
    : Attribute(index_, length_) {
  num_annotations = reader_->ReadUInt2();
  annotations = new Annotation *[num_annotations];
  for (u2 i = 0; i < num_annotations; ++i) {
    annotations[i] = new Annotation(reader_);
  }
}

RuntimeAnnotations_attribute::~RuntimeAnnotations_attribute() {
  delete[] annotations;
}

RuntimeInvisibleAnnotations_attribute::RuntimeInvisibleAnnotations_attribute(
    jvm::ClassReader *reader_, u2 index_, u4 length_)
    : RuntimeAnnotations_attribute(reader_, index_, length_) {}

template <typename R, typename D>
R RuntimeInvisibleAnnotations_attribute::Accept(AttributeVisitor<R, D> *v,
                                                D *data) {
  return v->Visit(this, data);
}

RuntimeVisibleAnnotations_attribute::RuntimeVisibleAnnotations_attribute(
    jvm::ClassReader *reader_, u2 index_, u4 length_)
    : RuntimeAnnotations_attribute(reader_, index_, length_) {}

template <typename R, typename D>
R RuntimeVisibleAnnotations_attribute::Accept(AttributeVisitor<R, D> *v,
                                              D *data) {
  return v->Visit(this, data);
}

MethodParameters_attribute::Entry::Entry(ClassReader *reader) {
  name_index = reader->ReadUInt2();
  access_flags = reader->ReadUInt2();
}
MethodParameters_attribute::MethodParameters_attribute(
    jvm::ClassReader *reader_, u2 index_, u4 length_)
    : Attribute(index_, length_) {
  parameters_count = reader_->ReadUInt1();
  parameters = new Entry *[parameters_count];
  for (u1 i = 0; i < parameters_count; ++i) {
    parameters[i] = new Entry(reader_);
  }
}
MethodParameters_attribute::~MethodParameters_attribute() {
  delete[] parameters;
}

Module_attribute::RequiresEntry::RequiresEntry(jvm::ClassReader *reader_) {
  requires_index = reader_->ReadUInt2();
  requires_flags = reader_->ReadUInt2();
  requires_version_index = reader_->ReadUInt2();
}
Module_attribute::ExportsEntry::ExportsEntry(jvm::ClassReader *reader_) {
  exports_index = reader_->ReadUInt2();
  exports_flags = reader_->ReadUInt2();
  exports_to_count = reader_->ReadUInt2();
  exports_to_index = new u2[exports_to_count];
  for (u2 i = 0; i < exports_to_count; ++i) {
    exports_to_index[i] = reader_->ReadUInt2();
  }
}
Module_attribute::ExportsEntry::~ExportsEntry() { delete exports_to_index; }
Module_attribute::OpensEntry::OpensEntry(jvm::ClassReader *reader_) {
  opens_index = reader_->ReadUInt2();
  opens_flags = reader_->ReadUInt2();
  opens_to_count = reader_->ReadUInt2();
  opens_to_index = new u2[opens_to_count];
  for (u2 i = 0; i < opens_to_count; ++i) {
    opens_to_index[i] = reader_->ReadUInt2();
  }
}
Module_attribute::OpensEntry::~OpensEntry() { delete opens_to_index; }
Module_attribute::ProvidesEntry::ProvidesEntry(jvm::ClassReader *reader_) {
  provides_index = reader_->ReadUInt2();
  provides_with_count = reader_->ReadUInt2();
  provides_with_index = new u2[provides_with_count];
  for (u2 i = 0; i < provides_with_count; ++i) {
    provides_with_index[i] = reader_->ReadUInt2();
  }
}
Module_attribute::ProvidesEntry::~ProvidesEntry() {
  delete provides_with_index;
}

Module_attribute::Module_attribute(jvm::ClassReader *reader_, u2 index_,
                                   u4 length_)
    : Attribute(index_, length_) {

  module_name_index = reader_->ReadUInt2();
  module_flags = reader_->ReadUInt2();
  module_version_index = reader_->ReadUInt2();

  requires_count = reader_->ReadUInt2();
  requires_entry = new RequiresEntry *[requires_count];
  for (int i = 0; i < requires_count; i++) {
    requires_entry[i] = new RequiresEntry(reader_);
  }

  exports_count = reader_->ReadUInt2();
  exports_entry = new ExportsEntry *[exports_count];
  for (int i = 0; i < exports_count; i++) {
    exports_entry[i] = new ExportsEntry(reader_);
  }

  opens_count = reader_->ReadUInt2();
  opens_entry = new OpensEntry *[opens_count];
  for (int i = 0; i < opens_count; i++) {
    opens_entry[i] = new OpensEntry(reader_);
  }

  uses_count = reader_->ReadUInt2();
  uses_index = new u2[uses_count];
  for (int i = 0; i < uses_count; i++) {
    uses_index[i] = reader_->ReadUInt2();
  }

  provides_count = reader_->ReadUInt2();
  provides_entry = new ProvidesEntry *[provides_count];
  for (int i = 0; i < provides_count; i++) {
    provides_entry[i] = new ProvidesEntry(reader_);
  }
}

Module_attribute::~Module_attribute() {
  delete[] requires_entry;
  delete[] exports_entry;
  delete uses_index;
  delete[] provides_entry;
  delete[] exports_entry;
}

template <typename R, typename D>
R Module_attribute::Accept(AttributeVisitor<R, D> *v, D *data) {
  return v->Visit(this, data);
}

ModulePackages_attribute::ModulePackages_attribute(jvm::ClassReader *reader_,
                                                   u2 index_, u4 length_)
    : Attribute(index_, length_) {
  package_count = reader_->ReadUInt2();
  package_index = new u2[package_count];
  for (u2 i = 0; i < package_count; ++i) {
    package_index[i] = reader_->ReadUInt2();
  }
}

ModulePackages_attribute::~ModulePackages_attribute() { delete package_index; }

template <typename R, typename D>
R ModulePackages_attribute::Accept(AttributeVisitor<R, D> *v, D *data) {
  return v->Visit(this, data);
}

ModuleMainClass_attribute::ModuleMainClass_attribute(jvm::ClassReader *reader_,
                                                     u2 index_, u4 length_)
    : Attribute(index_, length_) {
  main_class_index = reader_->ReadUInt2();
}

template <typename R, typename D>
R ModuleMainClass_attribute::Accept(AttributeVisitor<R, D> *v, D *data) {
  return v->Visit(this, data);
}

// endregion tools attribute

// endregion attribute

// region AttributeFactory

AttributeFactory::AttributeFactory(ClassFile *classFile_) {
  class_file = classFile_;
  Init();
}
AttributeFactory::~AttributeFactory() { attribute_table.clear(); }
Attribute *AttributeFactory::CreateAttribute(ClassReader *reader) {
  u2 name_index = reader->ReadUInt2();
  u4 length = reader->ReadInt4();
  std::string name = class_file->constant_pool->GetUTF8Value(name_index);
  auto attr_type = attribute_table[name];
  if (attr_type == 0) {
    throw InvalidIndex(name_index);
  }
  switch (attr_type) {
  case 1:
    return new AnnotationDefault_attribute(reader, name_index, length);
  case 2:
    return new BootstrapMethods_attribute(reader, name_index, length);
  case 4:
    return new Code_attribute(reader, name_index, length);
  case 5:
    return new ConstantValue_attribute(reader, name_index, length);
  case 7:
    return new Deprecated_attribute(reader, name_index, length);
  case 8:
    return new EnclosingMethod_attribute(reader, name_index, length);
  case 9:
    return new Exceptions_attribute(reader, name_index, length);
  case 10:
    return new InnerClasses_attribute(reader, name_index, length);
  case 11:
    return new LineNumberTable_attribute(reader, name_index, length);
  case 12:
    return new LocalVariableTable_attribute(reader, name_index, length);
  case 13:
    return new LocalVariableTypeTable_attribute(reader, name_index, length);
  case 14:
    return new MethodParameters_attribute(reader, name_index, length);
  case 15:
    return new Module_attribute(reader, name_index, length);
  case 17:
    return new ModuleMainClass_attribute(reader, name_index, length);
  case 18:
    return new ModulePackages_attribute(reader, name_index, length);
  case 21:
    return new NestHost_attribute(reader, name_index, length);
  case 22:
    return new NestMembers_attribute(reader, name_index, length);
  case 23:
    throw InvalidIndex(name_index);
  case 24:
    return new RuntimeVisibleAnnotations_attribute(reader, name_index, length);
  case 25:
    return new RuntimeInvisibleAnnotations_attribute(reader, name_index,
                                                     length);
    //  case 26:
    //    return new RuntimeVisibleParameterAnnotations_attribute(reader,
    //    name_index,
    //                                                            length);
    //  case 27:
    //    return new RuntimeInvisibleParameterAnnotations_attribute(
    //        reader, name_index, length);
    //  case 28:
    //    return new RuntimeVisibleTypeAnnotations_attribute(reader, name_index,
    //                                                       length);
    //  case 29:
    //    return new RuntimeInvisibleTypeAnnotations_attribute(reader,
    //    name_index,
    //                                                         length);
  case 30:
    return new Signature_attribute(reader, name_index, length);
  case 31:
    return new SourceDebugExtension_attribute(reader, name_index, length);
  case 32:
    return new SourceFile_attribute(reader, name_index, length);
  case 34:
    return new StackMapTable_attribute(reader, name_index, length);
  case 35:
    return new Synthetic_attribute(reader, name_index, length);
  default:
    throw InvalidIndex(name_index);
  }
}

void AttributeFactory::Init() {
  attribute_table.insert({AnnotationDefault, 1});
  attribute_table.insert({BootstrapMethods, 2});
  attribute_table.insert({CharacterRangeTable, 3});
  attribute_table.insert({Code, 4});
  attribute_table.insert({ConstantValue, 5});

  attribute_table.insert({CompilationID, 6});
  attribute_table.insert({Deprecated, 7});
  attribute_table.insert({EnclosingMethod, 8});
  attribute_table.insert({Exceptions, 9});
  attribute_table.insert({InnerClasses, 10});

  attribute_table.insert({LineNumberTable, 11});
  attribute_table.insert({LocalVariableTable, 12});
  attribute_table.insert({LocalVariableTypeTable, 13});
  attribute_table.insert({MethodParameters, 14});
  attribute_table.insert({Module, 15});

  attribute_table.insert({ModuleHashes, 16});
  attribute_table.insert({ModuleMainClass, 17});
  attribute_table.insert({ModulePackages, 18});
  attribute_table.insert({ModuleResolution, 19});
  attribute_table.insert({ModuleTarget, 20});

  attribute_table.insert({NestHost, 21});
  attribute_table.insert({NestMembers, 22});
  attribute_table.insert({Record, 23});
  attribute_table.insert({RuntimeVisibleAnnotations, 24});
  attribute_table.insert({RuntimeInvisibleAnnotations, 25});

  attribute_table.insert({RuntimeVisibleParameterAnnotations, 26});
  attribute_table.insert({RuntimeInvisibleParameterAnnotations, 27});
  attribute_table.insert({RuntimeVisibleTypeAnnotations, 28});
  attribute_table.insert({RuntimeInvisibleTypeAnnotations, 29});
  attribute_table.insert({Signature, 30});

  attribute_table.insert({SourceDebugExtension, 31});
  attribute_table.insert({SourceFile, 32});
  attribute_table.insert({SourceID, 33});
  attribute_table.insert({StackMapTable, 34});
  attribute_table.insert({Synthetic, 35});
}

// endregion AttributeFactory

// region Attributes

Attributes::Attributes(ClassReader *reader) {
  attributes_count = reader->ReadUInt2();
  attributes.reserve(attributes_count);
  auto cp = reader->GetClassFile()->constant_pool;
  for (u2 i = 0; i < attributes_count; ++i) {
    auto attr = reader->ReadAttribute();
    attributes.push_back(attr);
    try {
      std::string s = cp->GetUTF8Value(attr->attribute_name_index);
      map.insert({s, attr});
    } catch (const ConstantPoolException &e) {
    }
  }
}
Attributes::Attributes(u2 attributes_count_, Attribute **attributes_) {
  attributes_count = attributes_count_;
  attributes.reserve(attributes_count);
  for (u2 i = 0; i < attributes_count; ++i) {
    auto attr = attributes_[i];
    attributes.push_back(attr);
  }
}
Attributes::~Attributes() {
  for (auto &attr : attributes) {
    delete attr;
  }
  attributes.clear();
}
u4 Attributes::Size() { return attributes_count; }
u4 Attributes::Length() {
  u4 len = 2;

  return len;
}
Attribute *Attributes::Get(const std::string &name) { return map[name]; }
Attribute *Attributes::Get(u4 index) { return attributes[index]; }
// endregion Attributes

} // namespace jvm