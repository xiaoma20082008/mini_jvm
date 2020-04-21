//
// Created by machunxiao on 2020/4/16.
//

#ifndef MINI_JVM_CLASSFILE_H
#define MINI_JVM_CLASSFILE_H
#include "Common.h"
#include "classfile/AccessFlags.h"
#include "runtime/ConstantPool.h"

namespace jvm {

#define DEF_ATTR(name) struct name##_attribute : public Attribute

#define DST_ATTR(name) ~name##_attribute() override
#define DEF_ATTR_VISITOR(name)                                                 \
  virtual R Visit(const name##_attribute& attr, P* data) = 0
// end DEF_ATTR_VISITOR

#define DEF_VALUE_VISITOR(name)                                                \
  virtual R Visit(const name##_value* value, P* data) = 0
// end DEF_VALUE_VISITOR

#define DEF_VISIT_THIS(name)                                                   \
  template <typename R, typename P>                                            \
  R Accept(const name##Visitor<R, P>& v, P* data) {                            \
    return v.Visit(this, data);                                                \
  }
// end DEF_VISIT_THIS

template <typename R, typename P> struct AttrVisitor;
template <typename R, typename P> struct ValueVisitor;

struct Attribute {
  Attribute() : name_index(0), length(0) {}
  virtual ~Attribute() = default;
  u2 name_index = 0;
  u4 length = 0;
};

// region jvm attribute

DEF_ATTR(ConstantValue) { u2 constant_value_index; };

DEF_ATTR(Code) {

  struct exception_data {
    u2 start_pc = 0;
    u2 end_pc = 0;
    u2 handler_pc = 0;
    u2 catch_type = 0;
  };

  u2 max_stack = 0;
  u2 max_locals = 0;
  u4 code_length = 0;
  u1* code = nullptr;

  u2 exception_table_length = 0;
  exception_data** exception_tables = nullptr;
  u2 attributes_count = 0;
  Attribute** attributes = nullptr;

  DST_ATTR(Code) {
    DEL_PTR_ARRAY(exception_tables, exception_table_length);
    DEL_PTR_ARRAY(attributes, attributes_count);
  }
};

DEF_ATTR(StackMapTable) {

  // region verification_type_info

  static const u1 ITEM_Top = 0;
  static const u1 ITEM_Integer = 1;
  static const u1 ITEM_Float = 2;
  static const u1 ITEM_Double = 3;
  static const u1 ITEM_Long = 4;
  static const u1 ITEM_Null = 5;
  static const u1 ITEM_UninitializedThis = 6;
  static const u1 ITEM_Object = 7;
  static const u1 ITEM_Uninitialized = 8;

  struct verification_type_info {
    u1 tag = 0;
  };
  struct Object_variable_info : public verification_type_info {
    u2 const_pool_index = 0;

  private:
    using super = verification_type_info;
  };
  struct Uninitialized_variable_info : public verification_type_info {
    u2 offset = 0;

  private:
    using super = verification_type_info;
  };

  // endregion verification_type_info

  // region frame

  struct stack_map_frame {
    u1 frame_type = 0;
  };
  struct same_frame : public stack_map_frame {
    /* 0-63 */
  };
  struct same_locals_1_stack_item_frame : public stack_map_frame {
    /* 64-127 */
    // 1
    verification_type_info** stack = nullptr;

  private:
    using super = stack_map_frame;
  };
  struct same_locals_1_stack_item_frame_extended : public stack_map_frame {
    /* 247 */
    u2 offset_delta;
    // 1
    verification_type_info** stack = nullptr;

  private:
    using super = stack_map_frame;
  };
  struct chop_frame : public stack_map_frame {
    /* 248-250 */
    u2 offset_delta = 0;

  private:
    using super = stack_map_frame;
  };
  struct same_frame_extended : public stack_map_frame {
    /* 251 */
    u2 offset_delta = 0;

  private:
    using super = stack_map_frame;
  };
  struct append_frame : public stack_map_frame {
    /* 252-254 */
    u2 offset_delta;
    verification_type_info** stack = nullptr;

  private:
    using super = stack_map_frame;
  };
  struct full_frame : public stack_map_frame {
    /* 255 */
    u2 offset_delta = 0;
    u2 number_of_locals = 0;
    verification_type_info** locals = nullptr;
    u2 number_of_stack_items = 0;
    verification_type_info** stack = 0;

  private:
    using super = stack_map_frame;
  };

  // endregion frame

  DST_ATTR(StackMapTable) { delete[] entries; }
  u2 number_of_entries = 0;
  stack_map_frame** entries = nullptr;
};

DEF_ATTR(BootstrapMethods) {
  struct BootStrapMethod {
    u2 bootstrap_method_ref = 0;
    u2 num_bootstrap_arguments = 0;
    u2* bootstrap_arguments = nullptr;
  };
  u2 num_bootstrap_methods = 0;
  BootStrapMethod** bootstrap_methods = nullptr;

  DST_ATTR(BootstrapMethods) { delete[] bootstrap_methods; }
};

DEF_ATTR(NestHost) { u2 top_index = 0; };

DEF_ATTR(NestMembers) {
  u2 member_count = 0;
  u2* members_indexes = nullptr;
  DST_ATTR(NestMembers) { delete members_indexes; }
};

// endregion jvm attribute

// region java se attribute

DEF_ATTR(EnclosingMethod) {
  u2 class_index = 0;
  u2 method_index = 0;
  DEF_VISIT_THIS(Attr);
};

DEF_ATTR(Exceptions) {
  DST_ATTR(Exceptions) { delete exception_index_table; }
  u2 number_of_exceptions = 0;
  u2* exception_index_table = nullptr;
  DEF_VISIT_THIS(Attr);
};

DEF_ATTR(InnerClasses) {
  DST_ATTR(InnerClasses) { DEL_PTR_ARRAY(classes, number_of_classes); }
  struct Info {
    u2 inner_class_info_index = 0;
    u2 outer_class_info_index = 0;
    u2 inner_name_index = 0;
    u2 inner_class_access_flags = 0;
  };
  u2 number_of_classes = 0;
  Info** classes = nullptr;
  DEF_VISIT_THIS(Attr);
};

DEF_ATTR(LineNumberTable) {
  DST_ATTR(LineNumberTable) {
    DEL_PTR_ARRAY(line_number_table, line_number_table_length);
  }
  struct LineNumberInfo {
    u2 start_pc = 0;
    u2 line_number = 0;
  };
  u2 line_number_table_length = 0;
  LineNumberInfo** line_number_table = nullptr;
  DEF_VISIT_THIS(Attr);
};

DEF_ATTR(LocalVariableTable) {
  DST_ATTR(LocalVariableTable) { delete[] local_variable_table; }
  struct Entry {
    u2 start_pc = 0;
    u2 length = 0;
    u2 name_index = 0;
    u2 descriptor_index = 0;
    u2 index = 0;
  };

  u2 local_variable_table_length = 0;
  Entry** local_variable_table = nullptr;
  DEF_VISIT_THIS(Attr);
};

DEF_ATTR(LocalVariableTypeTable) {
  DST_ATTR(LocalVariableTypeTable) { delete[] local_variable_type_table; }
  struct Entry {
    u2 start_pc = 0;
    u2 length = 0;
    u2 name_index = 0;
    u2 signature_index = 0;
    u2 index = 0;
  };

  u2 local_variable_type_table_length = 0;
  Entry** local_variable_type_table = nullptr;
  DEF_VISIT_THIS(Attr);
};

DEF_ATTR(Signature) {
  u2 signature_index = 0;
  DEF_VISIT_THIS(Attr);
};

DEF_ATTR(SourceFile) {
  u2 sourcefile_index = 0;
  DEF_VISIT_THIS(Attr);
};

DEF_ATTR(Synthetic) { DEF_VISIT_THIS(Attr); };

// endregion java se attribute

// region tools attribute

DEF_ATTR(SourceDebugExtension) {
  DST_ATTR(SourceDebugExtension) { delete debug_extension; }
  u4 attribute_length = 0;
  u1* debug_extension = nullptr;
  DEF_VISIT_THIS(Attr);
};
DEF_ATTR(Deprecated) { DEF_VISIT_THIS(Attr); };

struct ElementValue {
  virtual ~ElementValue() = default;
  u1 tag;
};

struct Annotation {
  u2 typeIndex;
  u2 numElementValuePairs;

  struct ElementValuePairs {
    u2 elementNameIndex;
    ElementValue* value;

    ~ElementValuePairs() { delete value; }
  };

  ElementValuePairs** elementValuePairs;

  ~Annotation() { DEL_PTR_ARRAY(elementValuePairs, numElementValuePairs); }
};

struct Primitive_value : public ElementValue {
  u2 constValueIndex;
  DEF_VISIT_THIS(Value);
};
struct Enum_value : public ElementValue {
  u2 typeNameIndex;
  u2 constNameIndex;
  DEF_VISIT_THIS(Value);
};
struct Class_value : public ElementValue {
  u2 classInfoIndex;
  DEF_VISIT_THIS(Value);
};
struct Array_value : public ElementValue {
  u2 numValues;
  ElementValue** values;
  ~Array_value() override { DEL_PTR_ARRAY(values, numValues); }
  DEF_VISIT_THIS(Value);
};
struct Annotation_value : public ElementValue {
  Annotation* annotationValue;

  ~Annotation_value() override { delete annotationValue; }
  DEF_VISIT_THIS(Value);
};

DEF_ATTR(RuntimeAnnotations) {
  DST_ATTR(RuntimeAnnotations) { DEL_PTR_ARRAY(annotations, num_annotations); }
  u2 num_annotations = 0;
  Annotation** annotations = nullptr;
  DEF_VISIT_THIS(Attr);
};
DEF_ATTR(RuntimeInvisibleAnnotations) { DEF_VISIT_THIS(Attr); };
DEF_ATTR(RuntimeVisibleAnnotations) { DEF_VISIT_THIS(Attr); };
DEF_ATTR(RuntimeParameterAnnotations) {
  DST_ATTR(RuntimeParameterAnnotations) {
    DEL_PTR_ARRAY(parameter_annotations, num_parameters);
  }
  struct Entry {
    u2 num_annotations;
    Annotation** annotations;
  };

  u1 num_parameters;
  Annotation** parameter_annotations;
  DEF_VISIT_THIS(Attr);
};
DEF_ATTR(RuntimeInvisibleParameterAnnotations) { DEF_VISIT_THIS(Attr); };
DEF_ATTR(RuntimeVisibleParameterAnnotations) { DEF_VISIT_THIS(Attr); };
DEF_ATTR(RuntimeTypeAnnotations) { DEF_VISIT_THIS(Attr); };
DEF_ATTR(RuntimeInvisibleTypeAnnotations) { DEF_VISIT_THIS(Attr); };
DEF_ATTR(RuntimeVisibleTypeAnnotations) { DEF_VISIT_THIS(Attr); };
DEF_ATTR(AnnotationDefault) {
  DST_ATTR(AnnotationDefault) { delete value; }
  ElementValue* value = nullptr;
  DEF_VISIT_THIS(Attr);
};
DEF_ATTR(MethodParameters) {
  DST_ATTR(MethodParameters) { DEL_PTR_ARRAY(parameters, parameters_count); }
  struct Entry {
    u2 name_index;
    u2 access_flags;
  };
  u1 parameters_count = 0;
  Entry** parameters = nullptr;
  DEF_VISIT_THIS(Attr);
};
DEF_ATTR(Module) {

  DST_ATTR(Module) {
    DEL_PTR_ARRAY(requires_entry, requires_count);
    DEL_PTR_ARRAY(exports_entry, exports_count);
    DEL_PTR_ARRAY(opens_entry, opens_count);
    DEL_PTR_ARRAY(provides_entry, provides_count);
    delete uses_index;
  }
  DEF_VISIT_THIS(Attr);

  struct RequiresEntry {
    u2 requires_index = 0;
    u2 requires_flags = 0;
    u2 requires_version_index = 0;
  };
  struct ExportsEntry {
    u2 exports_index = 0;
    u2 exports_flags = 0;
    u2 exports_to_count = 0;
    u2* exports_to_index = nullptr;
    ~ExportsEntry() { delete exports_to_index; };
  };
  struct OpensEntry {
    u2 opens_index = 0;
    u2 opens_flags = 0;
    u2 opens_to_count = 0;
    u2* opens_to_index = nullptr;
    ~OpensEntry() { delete opens_to_index; };
  };
  struct ProvidesEntry {
    u2 provides_index = 0;
    u2 provides_with_count = 0;
    u2* provides_with_index = nullptr;
    ~ProvidesEntry() { delete provides_with_index; };
  };

  u2 module_name_index = 0;
  u2 module_flags = 0;
  u2 module_version_index = 0;

  u2 requires_count = 0;
  RequiresEntry** requires_entry = nullptr;

  u2 exports_count = 0;
  ExportsEntry** exports_entry = nullptr;

  u2 opens_count = 0;
  OpensEntry** opens_entry = nullptr;

  u2 uses_count = 0;
  u2* uses_index = nullptr;

  u2 provides_count = 0;
  ProvidesEntry** provides_entry = nullptr;
};
DEF_ATTR(ModulePackages) {
  DST_ATTR(ModulePackages) { delete package_index; }
  DEF_VISIT_THIS(Attr);

  u2 package_count = 0;
  u2* package_index = nullptr;
};
DEF_ATTR(ModuleMainClass) {
  u2 main_class_index = 0;
  DEF_VISIT_THIS(Attr);
};
DEF_ATTR(Default) { DEF_VISIT_THIS(Attr); };
DEF_ATTR(ModuleHashes) { DEF_VISIT_THIS(Attr); };
DEF_ATTR(ModuleResolution) {
  u2 resolution_flags;
  DEF_VISIT_THIS(Attr);
};
DEF_ATTR(ModuleTarget) {
  u2 target_platform_index;
  DEF_VISIT_THIS(Attr);
};
DEF_ATTR(Record) {
  DST_ATTR(Record) { DEL_PTR_ARRAY(component, component_count); }
  DEF_VISIT_THIS(Attr);

  struct ComponentInfo {
    u2 name_index = 0;
    u2 descriptor_index = 0;
    u2 attributes_count = 0;
    Attribute** attributes = nullptr;
    ~ComponentInfo() { delete[] attributes; }
  };

  u2 component_count;
  ComponentInfo** component;
};
DEF_ATTR(SourceID) {
  u2 sourceID_index;
  DEF_VISIT_THIS(Attr);
};
DEF_ATTR(StackMap) { DEF_VISIT_THIS(Attr); };

// endregion tools attribute

template <typename R, typename P> struct AttrVisitor {
  DEF_ATTR_VISITOR(ConstantValue);
  DEF_ATTR_VISITOR(Code);
  DEF_ATTR_VISITOR(StackMapTable);
  DEF_ATTR_VISITOR(BootstrapMethods);
  DEF_ATTR_VISITOR(NestHost);
  DEF_ATTR_VISITOR(NestMembers);

  DEF_ATTR_VISITOR(EnclosingMethod);
  DEF_ATTR_VISITOR(Exceptions);
  DEF_ATTR_VISITOR(InnerClasses);
  DEF_ATTR_VISITOR(LineNumberTable);
  DEF_ATTR_VISITOR(LocalVariableTable);
  DEF_ATTR_VISITOR(LocalVariableTypeTable);
  DEF_ATTR_VISITOR(Signature);
  DEF_ATTR_VISITOR(SourceFile);
  DEF_ATTR_VISITOR(Synthetic);

  DEF_ATTR_VISITOR(SourceDebugExtension);
  DEF_ATTR_VISITOR(Deprecated);
  DEF_ATTR_VISITOR(RuntimeAnnotations);
  DEF_ATTR_VISITOR(RuntimeInvisibleAnnotations);
  DEF_ATTR_VISITOR(RuntimeVisibleAnnotations);
  DEF_ATTR_VISITOR(RuntimeParameterAnnotations);
  DEF_ATTR_VISITOR(RuntimeInvisibleParameterAnnotations);
  DEF_ATTR_VISITOR(RuntimeVisibleParameterAnnotations);
  DEF_ATTR_VISITOR(RuntimeTypeAnnotations);
  DEF_ATTR_VISITOR(RuntimeInvisibleTypeAnnotations);
  DEF_ATTR_VISITOR(RuntimeVisibleTypeAnnotations);
  DEF_ATTR_VISITOR(AnnotationDefault);
  DEF_ATTR_VISITOR(MethodParameters);
  DEF_ATTR_VISITOR(Module);
  DEF_ATTR_VISITOR(ModulePackages);
  DEF_ATTR_VISITOR(ModuleMainClass);
  DEF_ATTR_VISITOR(Default);
  DEF_ATTR_VISITOR(ModuleHashes);
  DEF_ATTR_VISITOR(ModuleResolution);
  DEF_ATTR_VISITOR(ModuleTarget);
  DEF_ATTR_VISITOR(Record);
  DEF_ATTR_VISITOR(SourceID);
  DEF_ATTR_VISITOR(StackMap);
};

template <typename R, typename P> struct ValueVisitor {
  DEF_VALUE_VISITOR(Primitive);
  DEF_VALUE_VISITOR(Enum);
  DEF_VALUE_VISITOR(Class);
  DEF_VALUE_VISITOR(Annotation);
};

struct Method {

  ~Method() {
    delete access_flags;
    DEL_PTR_ARRAY(attributes, attributes_count);
  }

  void Call();

  TO_STRING();

  AccessFlags* access_flags = nullptr;
  u2 name_index = 0;
  u2 descriptor_index = 0;
  u2 attributes_count = 0;
  Attribute** attributes = nullptr;
};

struct Field {

  ~Field() {
    delete access_flags;
    DEL_PTR_ARRAY(attributes, attributes_count);
  }
  TO_STRING();

  AccessFlags* access_flags = nullptr;
  u2 name_index = 0;
  u2 descriptor_index = 0;
  u2 attributes_count = 0;
  Attribute** attributes = nullptr;
};

struct ClassFile {

  ~ClassFile() {
    delete constant_pool;
    delete access_flags;
    delete interfaces;
    DEL_PTR_ARRAY(fields, fields_count);
    DEL_PTR_ARRAY(methods, methods_count);
    DEL_PTR_ARRAY(attributes, attributes_count);
  }

  TO_STRING();

  u4 magic = 0;
  u2 minor_version = 0;
  u2 major_version = 0;
  ConstantPool* constant_pool = nullptr;
  AccessFlags* access_flags = nullptr;
  u2 this_class = 0;
  u2 super_class = 0;
  u2 interfaces_count = 0;
  u2* interfaces = nullptr;
  u2 fields_count = 0;
  Field** fields = nullptr;
  u2 methods_count = 0;
  Method** methods = nullptr;
  u2 attributes_count = 0;
  Attribute** attributes = nullptr;
};

} // namespace jvm
#endif // MINI_JVM_CLASSFILE_H
