//
// Created by machunxiao on 2020/4/17.
//

#ifndef MINI_JVM_MINIVM_H
#define MINI_JVM_MINIVM_H

namespace jvm {
class JvmStack;
class JvmHeap;
class JvmMethodArea;
class ConstantPool;
class ClassLoader;
class MiniVM {
public:
  MiniVM();
  ~MiniVM();
  void Run(int argc, char** args);

private:
  JvmStack* stack{};
  JvmHeap* heap{};
  JvmMethodArea* methodArea{};
  /// https://docs.oracle.com/javase/specs/jvms/se14/html/jvms-2.html#jvms-2.5.1
  ///
  void* pcRegister{};
  JvmStack* nativeStack{};
  ConstantPool* constantPool{};
  ClassLoader* classLoader{};
};
} // namespace jvm
#endif // MINI_JVM_MINIVM_H
