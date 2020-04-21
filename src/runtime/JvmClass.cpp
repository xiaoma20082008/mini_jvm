//
// Created by machunxiao on 2020/4/21.
//
#include "runtime/JvmClass.h"
namespace jvm {

void JvmClass::Parse() { raw = reader.Parse(); }

bool JvmClass::HasMethod(const std::string& name,
                         const std::string& desc) const {
  return GetMethod(name, desc) != nullptr;
}

Method* JvmClass::GetMethod(const std::string& name,
                            const std::string& desc) const {
  FOR_EACH(i, raw->methods_count) {
    auto method = raw->methods[i];
    auto mn = raw->constant_pool->GetUTF8Value(method->name_index);
    auto md = raw->constant_pool->GetUTF8Value(method->descriptor_index);
    if (mn == name && md == desc) {
      return method;
    }
  }
  return nullptr;
}

Field* JvmClass::GetField(const std::string& name) const {
  FOR_EACH(i, raw->fields_count) {
    auto field = raw->fields[i];
    auto fn = raw->constant_pool->GetUTF8Value(field->name_index);
    if (fn == name) {
      return field;
    }
  }
  return nullptr;
}

} // namespace jvm