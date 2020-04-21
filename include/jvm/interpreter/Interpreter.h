//
// Created by machunxiao on 2020/4/20.
//

#ifndef MINI_JVM_INTERPRETER_H
#define MINI_JVM_INTERPRETER_H
#include "memory/Allocation.h"
#include "runtime/JvmThread.h"

namespace jvm {
class BytecodeInterpreter : public StackObj {
public:
  void Run(JvmThread* thread);

private:
};
} // namespace jvm
#endif // MINI_JVM_INTERPRETER_H
