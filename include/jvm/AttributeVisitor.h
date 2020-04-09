//
// Created by machunxiao on 2020/4/7.
//

#ifndef MINI_JVM_ATTRIBUTEVISITOR_H
#define MINI_JVM_ATTRIBUTEVISITOR_H
namespace jvm {

// region JVM attribute

struct ConstantValue_attribute;
struct Code_attribute;
struct BootstrapMethods_attribute;
struct NestHost_attribute;
struct NestMembers_attribute;
struct StackMapTable_attribute;

// endregion JVM attribute

// region Java Se attribute

struct Exceptions_attribute;
struct InnerClasses_attribute;
struct EnclosingMethod_attribute;
struct Signature_attribute;
struct Synthetic_attribute;
struct SourceFile_attribute;
struct LineNumberTable_attribute;
struct LocalVariableTable_attribute;
struct LocalVariableTypeTable_attribute;

// endregion Java Se attribute

// region Tools attribute

struct Default_attribute;
struct AnnotationDefault_attribute;
struct CharacterRangeTable_attribute;
struct CompilationID_attribute;
struct Deprecated_attribute;
struct MethodParameters_attribute;
struct Module_attribute;
struct ModuleHashes_attribute;
struct ModuleMainClass_attribute;
struct ModulePackages_attribute;
struct ModuleResolution_attribute;
struct ModuleTarget_attribute;
struct Record_attribute;
struct RuntimeVisibleAnnotations_attribute;
struct RuntimeInvisibleAnnotations_attribute;
struct RuntimeVisibleParameterAnnotations_attribute;
struct RuntimeInvisibleParameterAnnotations_attribute;
struct RuntimeVisibleTypeAnnotations_attribute;
struct RuntimeInvisibleTypeAnnotations_attribute;
struct SourceDebugExtension_attribute;
struct SourceID_attribute;

// endregion Tools attribute

template <typename R, typename P> class AttributeVisitor {
  R Visit(BootstrapMethods_attribute *attr, P p);
  R Visit(Default_attribute *attr, P p);
  R Visit(AnnotationDefault_attribute *attr, P p);
  R Visit(CharacterRangeTable_attribute *attr, P p);
  R Visit(Code_attribute *attr, P p);
  R Visit(CompilationID_attribute *attr, P p);
  R Visit(ConstantValue_attribute *attr, P p);
  R Visit(Deprecated_attribute *attr, P p);
  R Visit(EnclosingMethod_attribute *attr, P p);
  R Visit(Exceptions_attribute *attr, P p);
  R Visit(InnerClasses_attribute *attr, P p);
  R Visit(LineNumberTable_attribute *attr, P p);
  R Visit(LocalVariableTable_attribute *attr, P p);
  R Visit(LocalVariableTypeTable_attribute *attr, P p);
  R Visit(MethodParameters_attribute *attr, P p);
  R Visit(Module_attribute *attr, P p);
  R Visit(ModuleHashes_attribute *attr, P p);
  R Visit(ModuleMainClass_attribute *attr, P p);
  R Visit(ModulePackages_attribute *attr, P p);
  R Visit(ModuleResolution_attribute *attr, P p);
  R Visit(ModuleTarget_attribute *attr, P p);
  R Visit(NestHost_attribute *attr, P p);
  R Visit(NestMembers_attribute *attr, P p);
  R Visit(Record_attribute *attr, P p);
  R Visit(RuntimeVisibleAnnotations_attribute *attr, P p);
  R Visit(RuntimeInvisibleAnnotations_attribute *attr, P p);
  R Visit(RuntimeVisibleParameterAnnotations_attribute *attr, P p);
  R Visit(RuntimeInvisibleParameterAnnotations_attribute *attr, P p);
  R Visit(RuntimeVisibleTypeAnnotations_attribute *attr, P p);
  R Visit(RuntimeInvisibleTypeAnnotations_attribute *attr, P p);
  R Visit(Signature_attribute *attr, P p);
  R Visit(SourceDebugExtension_attribute *attr, P p);
  R Visit(SourceFile_attribute *attr, P p);
  R Visit(SourceID_attribute *attr, P p);
  R Visit(StackMapTable_attribute *attr, P p);
  R Visit(Synthetic_attribute *attr, P p);
};

} // namespace jvm
#endif // MINI_JVM_ATTRIBUTEVISITOR_H
