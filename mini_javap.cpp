//
// Created by machunxiao on 2020/4/10.
//
#include "ClassFile.h"
#include <iostream>

int main(int argc, char **argv) {
  if (argc < 2) {
    return 0;
  }
  const char *class_file = argv[1];
  jvm::ClassFile cf(class_file);
  std::cout << cf.ToString() << std::endl;
  return 0;
}