//
// Created by machunxiao on 2020/4/21.
//
#include "classfile/ClassLoader.h"
#include "runtime/JvmClass.h"
#include <regex>
namespace jvm {

ClassLoader::~ClassLoader() {
  for (auto& pair : classes) {
    delete pair.second;
  }
  classes.clear();
}

JvmClass* ClassLoader::LoadClass(const std::string& className) {
  std::string file = classpath + "/" +
                     std::regex_replace(className, std::regex("\\."), "/") +
                     ".class";
  auto clazz = new JvmClass(file);
  classes.insert(std::make_pair(className, clazz));
  return clazz;
}

} // namespace jvm