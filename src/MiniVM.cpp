//
// Created by machunxiao on 2020/4/21.
//
#include "MiniVM.h"
#include "Common.h"
#include "classfile/ClassLoader.h"
#include "iostream"
#include "runtime/ConstantPool.h"
#include "runtime/JvmHeap.h"
#include "runtime/JvmMethodArea.h"
#include "runtime/JvmStack.h"
#include "vector"
namespace jvm {

MiniVM::MiniVM() = default;

MiniVM::~MiniVM() {
  delete stack;
  delete heap;
  delete methodArea;
  // delete pcRegister;
  delete nativeStack;
  delete constantPool;
}

void MiniVM::Run(int argc, char** args) {
  // java -cp classpath Main args
  std::string classpath;
  std::string main;
  std::vector<std::string> arguments;
  int offset;
  if (strcmp("-cp", args[0]) == 0) {
    classpath = std::string(args[1]);
    main = std::string(args[2]);
    offset = 3;
  } else {
    main = std::string(args[0]);
    offset = 1;
  }
  for (auto i = offset; i < argc; ++i) {
    arguments.emplace_back(args[i]);
  }
  classLoader = new ClassLoader(classpath);
  // 1.加载 2.验证 3.准备 4.解析 5.初始化 6.使用 7.卸载
  auto clazz = classLoader->LoadClass(main);
  auto method = clazz->GetMethod("main", "([Ljava/lang/String;)V");
  method->Call();
  std::cout << "done" << std::endl;
}

} // namespace jvm