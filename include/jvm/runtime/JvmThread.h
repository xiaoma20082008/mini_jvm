//
// Created by machunxiao on 2020/4/16.
//

#ifndef MINI_JVM_JVMTHREAD_H
#define MINI_JVM_JVMTHREAD_H
#include "runtime/JvmStack.h"
namespace jvm {
class JvmThread {
private:
  void* stack;
  void* vm;
};
} // namespace jvm
#endif // MINI_JVM_JVMTHREAD_H
