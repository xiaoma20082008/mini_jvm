//
// Created by machunxiao on 2020/4/4.
//
#include "ClassFile.h"
#include "ClassReader.h"
#include "ClassWriter.h"
#include <iostream>
namespace jvm {
using namespace std;
ClassFile::ClassFile(const char *file_) {

  reader = new ClassReader(file_, new AttributeFactory(this));

  magic = reader->ReadInt4();
  minor_version = reader->ReadUInt2();
  major_version = reader->ReadUInt2();

  cout << "magic                : " << magic << endl;
  cout << "minor_version        : " << minor_version << endl;
  cout << "major_version        : " << major_version << endl;

  constant_pool = new ConstantPool(reader);
  cout << "constant_pool_count  : " << constant_pool->Size() << endl;

  access_flags = reader->ReadUInt2();
  this_class = reader->ReadUInt2();
  super_class = reader->ReadUInt2();
  cout << "access_flags         : " << access_flags << endl;
  cout << "this_class           : " << this_class << endl;
  cout << "super_class          : " << super_class << endl;

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
  cout << "interfaces_count     : " << interfaces_count << endl;

  // fields
  fields_count = reader->ReadUInt2();
  fields = new Field *[fields_count];
  for (u2 i = 0; i < fields_count; ++i) {
    fields[i] = new Field(reader);
  }
  cout << "fields_count         : " << fields_count << endl;

  // methods
  methods_count = reader->ReadUInt2();
  methods = new Method *[methods_count];
  for (u2 i = 0; i < methods_count; ++i) {
    methods[i] = new Method(reader);
  }
  cout << "methods_count        : " << fields_count << endl;

  // attributes
  attributes = new Attributes(reader);
  cout << "attributes_count     : " << attributes->attributes_count << endl;
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
  access_flags = access_flags_;
  this_class = this_class_;
  super_class = super_class_;
  interfaces_count = interfaces_count_;
  interfaces = interfaces_;
  fields_count = fields_count_;
  fields = fields_;
  methods_count = methods_count_;
  methods = methods_;
  attributes = attributes_;
}

ClassFile::~ClassFile() {
  delete reader;
  delete constant_pool;
  delete interfaces;
  delete[] fields;
  delete[] methods;
  delete attributes;
}

} // namespace jvm