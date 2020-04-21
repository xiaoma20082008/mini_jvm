//
// Created by machunxiao on 2020/4/17.
//

#ifndef MINI_JVM_CLASSLOADER_H
#define MINI_JVM_CLASSLOADER_H
#include "Common.h"
#include "runtime/JvmClass.h"
#include <unordered_map>
#include <utility>
namespace jvm {
using namespace std;
class ClassLoader {
public:
  explicit ClassLoader(const char* path) : classpath(std::string(path)) {}
  explicit ClassLoader(std::string path) : classpath(std::move(path)) {}
  ~ClassLoader();
  JvmClass* LoadClass(const std::string& className);

private:
  std::string classpath;
  unordered_map<string, JvmClass*> classes;
};
} // namespace jvm
#endif // MINI_JVM_CLASSLOADER_H
