//
// Created by machunxiao on 2020/4/4.
//
#include "ClassFile.h"
#include "ClassReader.h"
#include "ClassWriter.h"
#include <iostream>
#include <regex>
#include <sstream>
namespace jvm {
using namespace std;
ClassFile::ClassFile(const char *file_) {

  reader = new ClassReader(file_, new AttributeFactory(this));

  magic = reader->ReadInt4();
  minor_version = reader->ReadUInt2();
  major_version = reader->ReadUInt2();

  constant_pool = new ConstantPool(reader);

  access_flags = new AccessFlags(reader->ReadUInt2());
  this_class = reader->ReadUInt2();
  super_class = reader->ReadUInt2();

  // interfaces
  interfaces_count = reader->ReadUInt2();
  if (interfaces_count == 0) {
    interfaces = nullptr;
  } else {
    interfaces = new u2[interfaces_count];
    for (u2 i = 0; i < interfaces_count; ++i) {
      interfaces[i] = reader->ReadUInt2();
    }
  }

  // fields
  fields_count = reader->ReadUInt2();
  fields.reserve(fields_count);
  for (u2 i = 0; i < fields_count; ++i) {
    fields.push_back(new Field(reader));
  }

  // methods
  methods_count = reader->ReadUInt2();
  methods.reserve(methods_count);
  for (u2 i = 0; i < methods_count; ++i) {
    methods.push_back(new Method(reader));
  }

  // attributes
  attributes = new Attributes(reader);

#ifdef PRINT_DEBUG
  assert(reader->file_size == reader->index);

  cout << "magic                : " << magic << endl;
  cout << "minor_version        : " << minor_version << endl;
  cout << "major_version        : " << major_version << endl;
  cout << "constant_pool_count  : " << constant_pool->Size() << endl;
  cout << "access_flags         : " << access_flags->flags << endl;
  cout << "this_class           : " << this_class << endl;
  cout << "super_class          : " << super_class << endl;
  cout << "interfaces_count     : " << interfaces_count << endl;
  cout << "fields_count         : " << fields_count << endl;
  for (auto &field : fields) {
    std::string name = field->GetName(constant_pool);
    cout << "--field              : " << name << endl;
    cout << "  field_access_flags : " << field->access_flags->flags << endl;
    cout << "  field_name_index   : " << field->name_index << endl;
    cout << "  field_desc_index   : " << field->descriptor_index << endl;
    for (auto &attr : field->attributes->attributes) {
      cout << "    attr             : " << attr->GetName(constant_pool) << endl;
    }
  }
  cout << "methods_count        : " << methods_count << endl;
  for (auto &method : methods) {
    std::string name = method->GetName(constant_pool);
    cout << "--method             : " << name << endl;
    cout << "  method_access_flags: " << method->access_flags->flags << endl;
    cout << "  method_name_index  : " << method->name_index << endl;
    cout << "  method_desc_index  : " << method->descriptor_index << endl;
    for (auto &attr : method->attributes->attributes) {
      cout << "    attr             : " << attr->GetName(constant_pool) << endl;
    }
  }
  cout << "attributes_count     : " << attributes->attributes_count << endl;
  for (auto &attr : attributes->attributes) {
    cout << "    attr             : " << attr->GetName(constant_pool) << endl;
  }
  cout << endl;
#endif
}

ClassFile::ClassFile(u4 magic_, u2 minor_version_, u2 major_version_,
                     ConstantPool *constant_pool_, u2 access_flags_,
                     u2 this_class_, u2 super_class_, u2 interfaces_count_,
                     u2 *interfaces_, u2 fields_count_, Field **fields_,
                     u2 methods_count_, Method **methods_,
                     Attributes *attributes_) {
  magic = magic_;
  minor_version = minor_version_;
  major_version = major_version_;
  constant_pool = constant_pool_;
  access_flags = new AccessFlags(access_flags_);
  this_class = this_class_;
  super_class = super_class_;
  interfaces_count = interfaces_count_;
  interfaces = interfaces_;
  fields_count = fields_count_;
  fields.reserve(fields_count);
  for (u2 i = 0; i < fields_count; ++i) {
    fields.push_back(fields_[i]);
  }
  methods_count = methods_count_;
  methods.reserve(methods_count);
  for (u2 i = 0; i < methods_count; ++i) {
    methods.push_back(methods_[i]);
  }
  attributes = attributes_;
}

ClassFile::~ClassFile() {
  delete reader;
  delete access_flags;
  delete constant_pool;
  delete interfaces;
  for (auto &field : fields) {
    delete field;
  }
  fields.clear();
  for (auto &method : methods) {
    delete method;
  }
  methods.clear();
  delete attributes;
}

bool ClassFile::IsClass() { return !IsInterface(); }
bool ClassFile::IsInterface() { return access_flags->Is(ACC_INTERFACE); }
std::string ClassFile::ToString() {
  std::stringstream ss;
  auto *attr = attributes->Get(SourceFile);
  auto *sfa = static_cast<SourceFile_attribute *>(attr);
  if (sfa != nullptr) {
    ss << "  Compiled from \"" << sfa->GetSourceFile(constant_pool) << "\""
       << endl;
  }
  for (auto &modifier : access_flags->GetClassModifiers()) {
    ss << modifier << " ";
  }
  if (IsClass()) {
    ss << "class ";
  } else if (IsInterface()) {
    ss << "interface ";
  }

  ss << std::regex_replace(constant_pool->GetClassInfo(this_class)->GetName(),
                           std::regex("/"), ".")
     << endl;
  ss << "  minor version: " << minor_version << endl;
  ss << "  major version: " << major_version << endl;
  ss << "  flags: " << access_flags->flags << endl;
  ss << "  this_class: #" << this_class << endl;
  ss << "  super_class: #" << super_class << endl;
  ss << "  interfaces: " << interfaces_count << ", fields: " << fields_count
     << ", methods: " << methods_count
     << ", attributes: " << attributes->attributes_count << endl;
  ss << "Constant pool:" << endl;
  ss << "{" << endl;
  ss << "}" << endl;
  return ss.str();
}

} // namespace jvm