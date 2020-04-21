//
// Created by machunxiao on 2020/4/21.
//
#include "classfile/ClassReader.h"

namespace jvm {

ClassFile* ClassReader::Parse() {
  auto raw = new ClassFile();
  raw->magic = reader.ReadInt4();
  raw->minor_version = reader.ReadUInt2();
  raw->major_version = reader.ReadUInt2();

  ParseConstantPool(raw, reader.ReadUInt2());

  raw->access_flags = new AccessFlags(reader.ReadUInt2());
  raw->this_class = reader.ReadUInt2();
  raw->super_class = reader.ReadUInt2();

  raw->interfaces_count = reader.ReadUInt2();
  ParseInterfaces(raw, raw->interfaces_count);

  raw->fields_count = reader.ReadUInt2();
  ParseFields(raw, raw->fields_count);

  raw->methods_count = reader.ReadUInt2();
  ParseMethods(raw, raw->methods_count);

  raw->attributes_count = reader.ReadUInt2();
  // raw->attributes = new Attribute*[raw->attributes_count];
  ParseAttributes(raw, raw->attributes, raw->attributes_count);
  return raw;
}

// region private

bool ClassReader::ParseConstantPool(ClassFile* raw, u2 constantPool_count_) {
  auto cp = new ConstantPool();
  cp->constant_pool_count = constantPool_count_;
  cp->pool = new CpInfo*[constantPool_count_];
  for (u2 i = 1; i < constantPool_count_; ++i) {
    u1 tag = reader.ReadUInt1();
    CpInfo* info = nullptr;
    switch (tag) {
    case CONSTANT_Utf8:
      info = new ConstantUtf8Info(cp, tag, reader.ReadUTF());
      break;
    case CONSTANT_Integer:
      info = new ConstantIntegerInfo(cp, tag, reader.ReadInt4());
      break;
    case CONSTANT_Float:
      info = new ConstantFloatInfo(cp, tag, reader.ReadInt4());
      break;
    case CONSTANT_Long: {
      auto high = reader.ReadInt4();
      auto low = reader.ReadInt4();
      info = new ConstantLongInfo(cp, tag, high, low);
      i++;
      break;
    }
    case CONSTANT_Double: {
      auto high = reader.ReadInt4();
      auto low = reader.ReadInt4();
      info = new ConstantDoubleInfo(cp, tag, high, low);
      i++;
      break;
    }
    case CONSTANT_Class:
      info = new ConstantClassInfo(cp, tag, reader.ReadUInt2());
      break;
    case CONSTANT_String:
      info = new ConstantStringInfo(cp, tag, reader.ReadUInt2());
      break;
    case CONSTANT_FieldRef: {
      auto cls_index = reader.ReadUInt2();
      auto type_index = reader.ReadUInt2();
      info = new ConstantFieldRefInfo(cp, tag, cls_index, type_index);
      break;
    }
    case CONSTANT_MethodRef: {
      auto cls_index = reader.ReadUInt2();
      auto type_index = reader.ReadUInt2();
      info = new ConstantMethodRefInfo(cp, tag, cls_index, type_index);
      break;
    }
    case CONSTANT_InterfaceMethodRef: {
      auto cls_index = reader.ReadUInt2();
      auto type_index = reader.ReadUInt2();
      info = new ConstantInterfaceMethodRefInfo(raw->constant_pool, tag,
                                                cls_index, type_index);
      break;
    }
    case CONSTANT_NameAndType: {
      auto name_index = reader.ReadUInt2();
      auto desc_index = reader.ReadUInt2();
      info = new ConstantNameAndTypeInfo(raw->constant_pool, tag, name_index,
                                         desc_index);
      break;
    }
    case CONSTANT_MethodHandle: {
      auto ref_kind = reader.ReadUInt1();
      auto ref_idx = reader.ReadUInt2();
      info = new ConstantMethodHandleInfo(raw->constant_pool, tag, ref_kind,
                                          ref_idx);
      break;
    }
    case CONSTANT_MethodType:
      info = new ConstantMethodTypeInfo(cp, tag, reader.ReadUInt2());
      break;
    case CONSTANT_Dynamic: {
      auto attr_idx = reader.ReadUInt2();
      auto type_idx = reader.ReadUInt2();
      info = new ConstantDynamicInfo(cp, tag, attr_idx, type_idx);
      break;
    }
    case CONSTANT_InvokeDynamic: {
      auto attr_idx = reader.ReadUInt2();
      auto type_idx = reader.ReadUInt2();
      info = new ConstantInvokeDynamicInfo(raw->constant_pool, tag, attr_idx,
                                           type_idx);
      break;
    }
    case CONSTANT_Module:
      info = new ConstantModuleInfo(cp, tag, reader.ReadUInt2());
      break;
    case CONSTANT_Package:
      info = new ConstantPackageInfo(cp, tag, reader.ReadUInt2());
      break;
    default:
      throw InvalidEntry(i, tag);
    }
    cp->pool[i] = info;
  }
  raw->constant_pool = cp;
  return true;
}
bool ClassReader::ParseInterfaces(ClassFile* raw, u2 interfaces_count_) {
  if (interfaces_count_ > 0) {
    raw->interfaces = new u2[interfaces_count_];
    FOR_EACH(i, interfaces_count_) { raw->interfaces[i] = reader.ReadUInt2(); }
  }
  return true;
}
bool ClassReader::ParseFields(ClassFile* raw, u2 fields_count_) {
  if (fields_count_ > 0) {
    raw->fields = new Field*[fields_count_];
    FOR_EACH(x, fields_count_) {
      auto field = new Field();
      field->access_flags = new AccessFlags(reader.ReadUInt2());
      field->name_index = reader.ReadUInt2();
      field->descriptor_index = reader.ReadUInt2();
      field->attributes_count = reader.ReadUInt2();
      ParseAttributes(raw, field->attributes, field->attributes_count);

      raw->fields[x] = field;
    }
  }
  return true;
}
bool ClassReader::ParseMethods(ClassFile* raw, u2 methods_count_) {
  if (methods_count_ > 0) {
    raw->methods = new Method*[methods_count_];
    FOR_EACH(x, methods_count_) {
      auto md = new Method();
      md->access_flags = new AccessFlags(reader.ReadUInt2());
      md->name_index = reader.ReadUInt2();
      md->descriptor_index = reader.ReadUInt2();
      md->attributes_count = reader.ReadUInt2();
      ParseAttributes(raw, md->attributes, md->attributes_count);

      raw->methods[x] = md;
    }
  }
  return true;
}
bool ClassReader::ParseAttributes(ClassFile* raw, Attribute**(&attr),
                                  u2 attributes_count_) {
  if (attributes_count_ > 0) {
    attr = new Attribute*[attributes_count_];
    FOR_EACH(i, attributes_count_) {
      u2 name_index = reader.ReadUInt2();
      u4 length = reader.ReadInt4();
      auto attr_desc = raw->constant_pool->GetUTF8Value(name_index);
      auto attr_type = attr_table[attr_desc];
      if (attr_type == 0) {
        throw InvalidIndex(name_index);
      }
      Attribute* attribute = nullptr;
      switch (attr_type) {
      case 1: {
        auto item = new AnnotationDefault_attribute();

        item->value = ParseElementValue(raw);
        attribute = item;
        break;
      }
      case 2: {
        auto item = new BootstrapMethods_attribute();
        item->num_bootstrap_methods = reader.ReadUInt2();
        item->bootstrap_methods = new BootstrapMethods_attribute::
            BootStrapMethod*[item->num_bootstrap_methods];
        FOR_EACH(j, item->num_bootstrap_methods) {
          auto m = new BootstrapMethods_attribute::BootStrapMethod();
          m->bootstrap_method_ref = reader.ReadUInt2();
          m->num_bootstrap_arguments = reader.ReadUInt2();
          m->bootstrap_arguments = new u2[m->num_bootstrap_arguments];
          FOR_EACH(x, m->num_bootstrap_arguments) {
            m->bootstrap_arguments[x] = reader.ReadUInt2();
          }
          item->bootstrap_methods[i] = m;
        }
        attribute = item;
        break;
      }
      case 4: {
        auto item = new Code_attribute();
        item->max_stack = reader.ReadUInt2();
        item->max_locals = reader.ReadUInt2();
        item->code_length = reader.ReadInt4();
        item->code = new u1[item->code_length];
        reader.ReadFully(item->code, 0, item->code_length);
        item->exception_table_length = reader.ReadUInt2();
        item->exception_tables =
            new Code_attribute::exception_data*[item->exception_table_length];
        FOR_EACH(x, item->exception_table_length) {
          auto ed = new Code_attribute::exception_data();
          ed->start_pc = reader.ReadUInt2();
          ed->end_pc = reader.ReadUInt2();
          ed->handler_pc = reader.ReadUInt2();
          ed->catch_type = reader.ReadUInt2();
          item->exception_tables[x] = ed;
        }
        item->attributes_count = reader.ReadUInt2();
        ParseAttributes(raw, item->attributes, item->attributes_count);
        attribute = item;
        break;
      }
      case 5: {
        auto item = new ConstantValue_attribute();
        item->constant_value_index = reader.ReadUInt2();
        attribute = item;
        break;
      }

      case 6:
        attribute = nullptr;
        break;
      case 7:
        attribute = new Deprecated_attribute();
        break;
      case 8:
        attribute = new EnclosingMethod_attribute();
        break;
      case 9:
        attribute = new Exceptions_attribute();
        break;
      case 10: {
        auto item = new InnerClasses_attribute();
        item->number_of_classes = reader.ReadUInt2();
        item->classes =
            new InnerClasses_attribute::Info*[item->number_of_classes];
        FOR_EACH(x, item->number_of_classes) {
          auto info = new InnerClasses_attribute::Info();
          info->inner_class_info_index = reader.ReadUInt2();
          info->outer_class_info_index = reader.ReadUInt2();
          info->inner_name_index = reader.ReadUInt2();
          info->inner_class_access_flags = reader.ReadUInt2();
          item->classes[x] = info;
        }
        attribute = item;
        break;
      }
      case 11: {
        auto item = new LineNumberTable_attribute();
        item->line_number_table_length = reader.ReadUInt2();
        item->line_number_table = new LineNumberTable_attribute::
            LineNumberInfo*[item->line_number_table_length];
        FOR_EACH(x, item->line_number_table_length) {
          auto tb = new LineNumberTable_attribute::LineNumberInfo();
          tb->start_pc = reader.ReadUInt2();
          tb->line_number = reader.ReadUInt2();
          item->line_number_table[x] = tb;
        }
        attribute = item;
        break;
      }
      case 12:
        attribute = new LocalVariableTable_attribute();
        break;
      case 13:
        attribute = new LocalVariableTypeTable_attribute();
        break;
      case 14:
        attribute = new MethodParameters_attribute();
        break;
      case 15:
        attribute = new Module_attribute();
        break;
      case 16:
        throw InvalidIndex(name_index);

      case 17:
        attribute = new ModuleMainClass_attribute();
        break;
      case 18:
        attribute = new ModulePackages_attribute();
        break;
      case 19:
      case 20:
        throw InvalidIndex(name_index);

      case 21:
        attribute = new NestHost_attribute();
        break;
      case 22:
        attribute = new NestMembers_attribute();
        break;
      case 23:
        throw InvalidIndex(name_index);
        break;
      case 24:
        attribute = new RuntimeVisibleAnnotations_attribute();
        break;
      case 25:
        attribute = new RuntimeInvisibleAnnotations_attribute();
        break;

      case 30:
        attribute = new Signature_attribute();
        break;
      case 31:
        attribute = new SourceDebugExtension_attribute();
        break;
      case 32: {
        auto item = new SourceFile_attribute();
        item->sourcefile_index = reader.ReadUInt2();
        attribute = item;
        break;
      }
      case 34:
        attribute = new StackMapTable_attribute();
        break;
      case 35:
        attribute = new Synthetic_attribute();
      default:
        throw InvalidIndex(name_index);
      }
      attribute->name_index = name_index;
      attribute->length = length;
      attr[i] = attribute;
    }
  }
  return true;
}

ElementValue* ClassReader::ParseElementValue(ClassFile* raw) {
  u1 tag = reader.ReadUInt1();
  switch (tag) {
  case 'B':
  case 'C':
  case 'D':
  case 'F':
  case 'I':
  case 'J':
  case 'S':
  case 'Z':
  case 's': {
    auto v = new Primitive_value();
    v->tag = tag;
    v->constValueIndex = reader.ReadUInt2();
    return v;
  }
  case 'e': {
    auto v = new Enum_value();
    v->tag = tag;
    v->typeNameIndex = reader.ReadUInt2();
    v->constNameIndex = reader.ReadUInt2();
    return v;
  }
  case 'c': {
    auto v = new Class_value();
    v->tag = tag;
    v->classInfoIndex = reader.ReadUInt2();
    return v;
  }

  case '@': {
    auto v = new Annotation_value();
    v->tag = tag;
    auto annotation = new Annotation();
    annotation->typeIndex = reader.ReadUInt2();
    annotation->numElementValuePairs = reader.ReadUInt2();
    annotation->elementValuePairs =
        new Annotation::ElementValuePairs*[annotation->numElementValuePairs];
    FOR_EACH(i, annotation->numElementValuePairs) {
      auto pair = new Annotation::ElementValuePairs();
      pair->elementNameIndex = reader.ReadUInt2();
      pair->value = ParseElementValue(raw);
      annotation->elementValuePairs[i] = pair;
    }
    v->annotationValue = annotation;
    return v;
  }

  case '[': {
    auto v = new Array_value();
    v->numValues = reader.ReadUInt2();
    if (v->numValues > 0) {
      v->values = new ElementValue*[v->numValues];
      FOR_EACH(i, v->numValues) { v->values[i] = ParseElementValue(raw); }
    }
    return v;
  }
  default:
    std::string s("unrecognized tag: ");
    s.append(std::to_string(tag));
    throw InvalidAnnotation(s);
  }
}

void ClassReader::InitMap() {
  attr_table.insert({AnnotationDefault, 1});
  attr_table.insert({BootstrapMethods, 2});
  attr_table.insert({CharacterRangeTable, 3});
  attr_table.insert({Code, 4});
  attr_table.insert({ConstantValue, 5});

  attr_table.insert({CompilationID, 6});
  attr_table.insert({Deprecated, 7});
  attr_table.insert({EnclosingMethod, 8});
  attr_table.insert({Exceptions, 9});
  attr_table.insert({InnerClasses, 10});

  attr_table.insert({LineNumberTable, 11});
  attr_table.insert({LocalVariableTable, 12});
  attr_table.insert({LocalVariableTypeTable, 13});
  attr_table.insert({MethodParameters, 14});
  attr_table.insert({Module, 15});

  attr_table.insert({ModuleHashes, 16});
  attr_table.insert({ModuleMainClass, 17});
  attr_table.insert({ModulePackages, 18});
  attr_table.insert({ModuleResolution, 19});
  attr_table.insert({ModuleTarget, 20});

  attr_table.insert({NestHost, 21});
  attr_table.insert({NestMembers, 22});
  attr_table.insert({Record, 23});
  attr_table.insert({RuntimeVisibleAnnotations, 24});
  attr_table.insert({RuntimeInvisibleAnnotations, 25});

  attr_table.insert({RuntimeVisibleParameterAnnotations, 26});
  attr_table.insert({RuntimeInvisibleParameterAnnotations, 27});
  attr_table.insert({RuntimeVisibleTypeAnnotations, 28});
  attr_table.insert({RuntimeInvisibleTypeAnnotations, 29});
  attr_table.insert({Signature, 30});

  attr_table.insert({SourceDebugExtension, 31});
  attr_table.insert({SourceFile, 32});
  attr_table.insert({SourceID, 33});
  attr_table.insert({StackMapTable, 34});
  attr_table.insert({Synthetic, 35});
}

// endregion private
} // namespace jvm