//
// Created by machunxiao on 2020/4/6.
//

#ifndef MINI_JVM_ATTRIBUTE_H
#define MINI_JVM_ATTRIBUTE_H
#include "Annotation.h"
#include "Common.h"
#include <unordered_map>
#include <vector>
namespace jvm {

// region attribute

class ClassReader;
class ClassWriter;
class ClassFile;
class Attributes;
class ConstantPool;
template <typename R, typename P> class AttributeVisitor;
/**
 * total = 6 + 9 + 13 = 28
 *
 * https://docs.oracle.com/javase/specs/jvms/se14/html/jvms-4.html#jvms-4.7
 *
 * 1. Six attributes are critical to correct interpretation of the class file
 *      by the Java Virtual Machine:
 *    ConstantValue
 *    Code
 *    StackMapTable
 *    BootstrapMethods
 *    NestHost
 *    NestMembers
 * 2. Nine attributes are critical to correct interpretation of the class file
 *      by the class libraries of the Java SE platform:
 *    Exceptions
 *    InnerClasses
 *    EnclosingMethod
 *    Synthetic
 *    Signature
 *    SourceFile
 *    LineNumberTable
 *    LocalVariableTable
 *    LocalVariableTypeTable
 * 3. Thirteen attributes are not critical to correct interpretation of the
 *      class file by either the Java Virtual Machine or the class libraries of
 *      the Java SE platform, but are useful for tools:
 *    SourceDebugExtension
 *    Deprecated
 *    RuntimeVisibleAnnotations
 *    RuntimeInvisibleAnnotations
 *    RuntimeVisibleParameterAnnotations
 *    RuntimeInvisibleParameterAnnotations
 *    RuntimeVisibleTypeAnnotations
 *    RuntimeInvisibleTypeAnnotations
 *    AnnotationDefault
 *    MethodParameters
 *    Module
 *    ModulePackages
 *    ModuleMainClass
 *
 */

struct Attribute {
public:
  Attribute(u2 index_, u4 length_);
  u4 Length();
  std::string GetName(ConstantPool *pool);
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);

public:
  static Attribute *Read(ClassReader *reader_);

public:
  u2 attribute_name_index = 0;
  u4 attribute_length = 0;
};

// region jvm attribute

struct ConstantValue_attribute : public Attribute {
  ConstantValue_attribute(ClassReader *reader_, u2 index_, u4 length_);
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);
  u2 constant_value_index;
};

struct Code_attribute : public Attribute {
  struct exception_data {
    explicit exception_data(ClassReader *reader_);
    u2 start_pc = 0;
    u2 end_pc = 0;
    u2 handler_pc = 0;
    u2 catch_type = 0;
  };

  Code_attribute(ClassReader *reader_, u2 index_, u4 length);
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);

  u2 max_stack = 0;
  u2 max_locals = 0;
  u4 code_length = 0;
  u1 *code = nullptr;
  u2 exception_table_length = 0;

  exception_data **exception_tables = nullptr;
  // u2 attributes_count;
  Attributes *attributes = nullptr;
};

struct StackMapTable_attribute : public Attribute {

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
    verification_type_info(u1 tag_);
    u2 Length();
    u1 tag = 0;

    static StackMapTable_attribute::verification_type_info *
    Read(ClassReader *reader_);
  };
  struct Object_variable_info : public verification_type_info {
    Object_variable_info(ClassReader *reader_);
    u2 Length();
    u2 cpool_index = 0;

  private:
    using super = verification_type_info;
  };
  struct Uninitialized_variable_info : public verification_type_info {
    Uninitialized_variable_info(ClassReader *reader_);
    u2 Length();
    u2 offset = 0;

  private:
    using super = verification_type_info;
  };

  // endregion verification_type_info

  // region frame

  struct stack_map_frame {
    virtual u1 Length() = 0;
    virtual u2 GetOffsetDelta() = 0;
    stack_map_frame(u1 frame_type_);
    u1 frame_type = 0;
  };
  struct same_frame : public stack_map_frame {
    /* 0-63 */
    same_frame(u1 tag_);
    u1 Length() override;
    u2 GetOffsetDelta() override;
  };
  struct same_locals_1_stack_item_frame : public stack_map_frame {
    /* 64-127 */
    same_locals_1_stack_item_frame(u1 tag_, ClassReader *reader_);
    ~same_locals_1_stack_item_frame();
    u1 Length() override;
    u2 GetOffsetDelta() override;
    // 1
    verification_type_info **stack = nullptr;

  private:
    using super = stack_map_frame;
  };
  struct same_locals_1_stack_item_frame_extended : public stack_map_frame {
    /* 247 */
    same_locals_1_stack_item_frame_extended(u1 tag_, ClassReader *reader_);
    ~same_locals_1_stack_item_frame_extended();
    u1 Length() override;
    u2 GetOffsetDelta() override;
    u2 offset_delta;
    // 1
    verification_type_info **stack = nullptr;

  private:
    using super = stack_map_frame;
  };
  struct chop_frame : public stack_map_frame {
    /* 248-250 */
    chop_frame(u1 tag_, ClassReader *reader_);
    u1 Length() override;
    u2 GetOffsetDelta() override;
    u2 offset_delta = 0;

  private:
    using super = stack_map_frame;
  };
  struct same_frame_extended : public stack_map_frame {
    /* 251 */
    same_frame_extended(u1 tag_, ClassReader *reader_);
    u1 Length() override;
    u2 GetOffsetDelta() override;
    u2 offset_delta = 0;

  private:
    using super = stack_map_frame;
  };
  struct append_frame : public stack_map_frame {
    /* 252-254 */
    append_frame(u1 tag_, ClassReader *reader_);
    ~append_frame();
    u1 Length() override;
    u2 GetOffsetDelta() override;
    u2 offset_delta;
    verification_type_info **stack = nullptr;

  private:
    using super = stack_map_frame;
  };
  struct full_frame : public stack_map_frame {
    /* 255 */
    full_frame(u1 tag_, ClassReader *reader_);
    ~full_frame();
    u1 Length() override;
    u2 GetOffsetDelta() override;

    u2 offset_delta = 0;
    u2 number_of_locals = 0;
    verification_type_info **locals = nullptr;
    u2 number_of_stack_items = 0;
    verification_type_info **stack = 0;

  private:
    using super = stack_map_frame;
  };

  // endregion frame

  StackMapTable_attribute(ClassReader *reader_, u2 index_, u4 length_);

  ~StackMapTable_attribute();

  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);

  u2 number_of_entries = 0;
  stack_map_frame **entries = nullptr;

private:
  static StackMapTable_attribute::stack_map_frame *Read(ClassReader *reader_);
};

struct BootstrapMethods_attribute : public Attribute {
  struct BootStrapMethod {
    u2 bootstrap_method_ref = 0;
    u2 num_bootstrap_arguments = 0;
    u2 *bootstrap_arguments = nullptr;
    BootStrapMethod(ClassReader *reader_);
  };
  BootstrapMethods_attribute(ClassReader *reader_, u2 index_, u4 length_);
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);

  u2 num_bootstrap_methods = 0;
  BootStrapMethod **bootstrap_methods = nullptr;
};

struct NestHost_attribute : public Attribute {
  NestHost_attribute(ClassReader *reader_, u2 index_, u4 length_);
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);
  u2 top_index = 0;
};

struct NestMembers_attribute : public Attribute {
  NestMembers_attribute(ClassReader *reader_, u2 index_, u4 length_);
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);
  u2 member_count = 0;
  u2 *members_indexes = nullptr;
};

// endregion jvm attribute

// region java se attribute

struct EnclosingMethod_attribute : public Attribute {
  EnclosingMethod_attribute(ClassReader *reader_, u2 index_, u4 length_);
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);
  u2 class_index = 0;
  u2 method_index = 0;
};

struct Exceptions_attribute : public Attribute {
  Exceptions_attribute(ClassReader *reader_, u2 index_, u4 length_);
  ~Exceptions_attribute();
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);

  u2 number_of_exceptions = 0;
  u2 *exception_index_table = nullptr;
};

struct InnerClasses_attribute : public Attribute {
  struct Info {
    u2 inner_class_info_index = 0;
    u2 outer_class_info_index = 0;
    u2 inner_name_index = 0;
    u2 inner_class_access_flags = 0;
    explicit Info(ClassReader *reader_);
  };
  InnerClasses_attribute(ClassReader *reader_, u2 index_, u4 length_);
  ~InnerClasses_attribute();
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);

  u2 number_of_classes = 0;
  Info **classes = nullptr;
};

struct LineNumberTable_attribute : public Attribute {
  struct LineNumberInfo {
    u2 start_pc = 0;
    u2 line_number = 0;
    explicit LineNumberInfo(ClassReader *reader_);
  };
  LineNumberTable_attribute(ClassReader *reader_, u2 index_, u4 length_);
  ~LineNumberTable_attribute();
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);

  u2 line_number_table_length = 0;
  LineNumberInfo **line_number_table = nullptr;
};

struct LocalVariableTable_attribute : public Attribute {
  struct Entry {
    u2 start_pc = 0;
    u2 length = 0;
    u2 name_index = 0;
    u2 descriptor_index = 0;
    u2 index = 0;
    Entry(ClassReader *reader_);
  };
  LocalVariableTable_attribute(ClassReader *reader_, u2 index_, u4 length_);
  ~LocalVariableTable_attribute();

  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);

  u2 local_variable_table_length = 0;
  Entry **local_variable_table = nullptr;
};

struct LocalVariableTypeTable_attribute : public Attribute {
  struct Entry {
    u2 start_pc = 0;
    u2 length = 0;
    u2 name_index = 0;
    u2 signature_index = 0;
    u2 index = 0;
    Entry(ClassReader *reader_);
  };
  LocalVariableTypeTable_attribute(ClassReader *reader_, u2 index_, u4 length_);
  ~LocalVariableTypeTable_attribute();
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);

  u2 local_variable_type_table_length = 0;
  Entry **local_variable_type_table = nullptr;
};

struct Signature_attribute : public Attribute {
  Signature_attribute(ClassReader *reader_, u2 index_, u4 length_);
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);
  u2 signature_index = 0;
};

struct SourceFile_attribute : public Attribute {
  SourceFile_attribute(ClassReader *reader_, u2 index_, u4 length_);
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);
  u2 sourcefile_index = 0;
  std::string GetSourceFile(ConstantPool *pool);
};

struct Synthetic_attribute : public Attribute {
  Synthetic_attribute(ClassReader *reader_, u2 index_, u4 length_);
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);
};

// endregion java se attribute

// region tools attribute

struct SourceDebugExtension_attribute : public Attribute {
  SourceDebugExtension_attribute(ClassReader *reader_, u2 index_, u4 length_);
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);
  std::string GetValue();

  u4 attribute_length = 0;
  u1 *debug_extension = nullptr;
};

struct Deprecated_attribute : public Attribute {
  Deprecated_attribute(ClassReader *reader_, u2 index_, u4 length_);
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);
};

struct RuntimeAnnotations_attribute : public Attribute {
  RuntimeAnnotations_attribute(ClassReader *reader_, u2 index_, u4 length_);
  ~RuntimeAnnotations_attribute();
  u2 num_annotations = 0;
  Annotation **annotations = nullptr;
};
struct RuntimeInvisibleAnnotations_attribute
    : public RuntimeAnnotations_attribute {
  RuntimeInvisibleAnnotations_attribute(ClassReader *reader_, u2 index_,
                                        u4 length_);
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);
};
struct RuntimeVisibleAnnotations_attribute
    : public RuntimeAnnotations_attribute {
  RuntimeVisibleAnnotations_attribute(ClassReader *reader_, u2 index_,
                                      u4 length_);
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);
};

/*
struct RuntimeParameterAnnotations_attribute : public Attribute {

  struct Entry {
    u2 num_annotations;
    Annotation **annotations;
  };

  RuntimeParameterAnnotations_attribute(ClassReader *reader_, u2 index_,
                                        u4 length_);
  ~RuntimeParameterAnnotations_attribute();

  u1 num_parameters;
  Annotation **parameter_annotations;
};

struct RuntimeInvisibleParameterAnnotations_attribute
    : public RuntimeParameterAnnotations_attribute {
  RuntimeInvisibleParameterAnnotations_attribute(ClassReader *reader_,
                                                 u2 index_, u4 length_);
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);
};
struct RuntimeVisibleParameterAnnotations_attribute
    : public RuntimeParameterAnnotations_attribute {
  RuntimeVisibleParameterAnnotations_attribute(ClassReader *reader_, u2 index_,
                                               u4 length_);
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);
};

struct RuntimeTypeAnnotations_attribute : public Attribute {};
struct RuntimeInvisibleTypeAnnotations_attribute
    : public RuntimeTypeAnnotations_attribute {
  RuntimeInvisibleTypeAnnotations_attribute(ClassReader *reader_, u2 index_,
                                            u4 length_);
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);
};
struct RuntimeVisibleTypeAnnotations_attribute
    : public RuntimeTypeAnnotations_attribute {
  RuntimeVisibleTypeAnnotations_attribute(ClassReader *reader_, u2 index_,
                                          u4 length_);
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);
};
*/

struct AnnotationDefault_attribute : public Attribute {
  AnnotationDefault_attribute(ClassReader *reader_, u2 index_, u4 length_);
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);
  element_value *value = nullptr;
};

struct MethodParameters_attribute : public Attribute {
  struct Entry {
    Entry(ClassReader *reader);
    u2 name_index;
    u2 access_flags;
  };
  MethodParameters_attribute(ClassReader *reader_, u2 index_, u4 length_);
  ~MethodParameters_attribute();
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);

  u1 parameters_count = 0;
  Entry **parameters = nullptr;
};

struct Module_attribute : public Attribute {
  struct RequiresEntry {
    u2 requires_index = 0;
    u2 requires_flags = 0;
    u2 requires_version_index = 0;
    RequiresEntry(ClassReader *reader_);
  };
  struct ExportsEntry {
    u2 exports_index = 0;
    u2 exports_flags = 0;
    u2 exports_to_count = 0;
    u2 *exports_to_index = nullptr;
    ExportsEntry(ClassReader *reader_);
    ~ExportsEntry();
  };
  struct OpensEntry {
    u2 opens_index = 0;
    u2 opens_flags = 0;
    u2 opens_to_count = 0;
    u2 *opens_to_index = nullptr;
    OpensEntry(ClassReader *reader_);
    ~OpensEntry();
  };
  struct ProvidesEntry {
    u2 provides_index = 0;
    u2 provides_with_count = 0;
    u2 *provides_with_index = nullptr;
    ProvidesEntry(ClassReader *reader_);
    ~ProvidesEntry();
  };
  Module_attribute(ClassReader *reader_, u2 index_, u4 length_);
  ~Module_attribute();
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);

  u2 module_name_index = 0;
  u2 module_flags = 0;
  u2 module_version_index = 0;

  u2 requires_count = 0;
  RequiresEntry **requires_entry = nullptr;

  u2 exports_count = 0;
  ExportsEntry **exports_entry = nullptr;

  u2 opens_count = 0;
  OpensEntry **opens_entry = nullptr;

  u2 uses_count = 0;
  u2 *uses_index = nullptr;

  u2 provides_count = 0;
  ProvidesEntry **provides_entry = nullptr;
};

struct ModulePackages_attribute : public Attribute {
  ModulePackages_attribute(ClassReader *reader_, u2 index_, u4 length_);
  ~ModulePackages_attribute();
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);
  u2 package_count = 0;
  u2 *package_index = nullptr;
};

struct ModuleMainClass_attribute : public Attribute {
  ModuleMainClass_attribute(ClassReader *reader_, u2 index_, u4 length_);
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);
  u2 main_class_index = 0;
};

/*
struct Default_attribute : public Attribute {
  Default_attribute(ClassReader *reader_, u2 index_, u4 length_);
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);
};

struct ModuleHashes_attribute : public Attribute {
  ModuleHashes_attribute(ClassReader *reader_, u2 index_, u4 length_);
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);
};

struct ModuleResolution_attribute : public Attribute {
  ModuleResolution_attribute(ClassReader *reader_, u2 index_, u4 length_);
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);
  u2 resolution_flags;
};
struct ModuleTarget_attribute : public Attribute {
  ModuleTarget_attribute(ClassReader *reader_, u2 index_, u4 length_);
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);
  u2 target_platform_index;
};

struct Record_attribute : public Attribute {
  struct ComponentInfo {
    u2 name_index;
    u2 descriptor_index;
    Attributes *attributes;
  };
  Record_attribute(ClassReader *reader_, u2 index_, u4 length_);
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);

  u2 component_count;
  ComponentInfo **component;
};

struct SourceID_attribute : public Attribute {
  SourceID_attribute(ClassReader *reader_, u2 index_, u4 length_);
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);

  u2 sourceID_index;
};
struct StackMap_attribute : public Attribute {
  StackMap_attribute(ClassReader *reader_, u2 index_, u4 length_);
  template <typename R, typename P>
  R Accept(AttributeVisitor<R, P> *v, P *data);
};
*/

// endregion tools attribute

// region attributes

class Attributes {
public:
  Attributes(u2 attributes_count_, Attribute **attributes_);
  Attributes(ClassReader *reader_);

  ~Attributes();
  u4 Size();
  u4 Length();
  Attribute *Get(u4 index);
  Attribute *Get(const std::string &name);

public:
  u2 attributes_count = 0;
  std::vector<Attribute *> attributes;
  std::unordered_map<std::string, Attribute *> map;
};

// endregion attributes

class AttributeFactory {
public:
  AttributeFactory(ClassFile *classFile_);
  ~AttributeFactory();

public:
  Attribute *CreateAttribute(ClassReader *reader_);

private:
  void Init();

public:
  ClassFile *class_file = nullptr;

private:
  std::unordered_map<std::string, u2> attribute_table;
};

// endregion attribute

} // namespace jvm
#endif // MINI_JVM_ATTRIBUTE_H
