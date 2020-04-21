#include "MiniVM.h"
/**
 * jvm启动程序
 * @param argc
 * @param argv
 * @return 0
 */
int main(int argc, char** argv) {
  jvm::MiniVM vm;
  int arg = argc - 1;
  char** cpy = new char*[arg]();
  for (auto i = 1; i < argc; ++i) {
    cpy[i - 1] = argv[i];
  }
  vm.Run(arg, cpy);
  delete[] cpy;
  return 0;
}
