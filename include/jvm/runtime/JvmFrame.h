//
// Created by machunxiao on 2020/4/21.
//

#ifndef MINI_JVM_JVMFRAME_H
#define MINI_JVM_JVMFRAME_H
namespace jvm {
class JvmFrame {
private:
  void* localVariables;
  void* operandStacks;
  void* constantPool;
};
} // namespace jvm
#endif // MINI_JVM_JVMFRAME_H
