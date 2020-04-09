//
// Created by machunxiao on 2020/4/8.
//
#include "ClassFile.h"
#include <gtest/gtest.h>
#include <iostream>

TEST(class_file, hello_world) {
  const char *path = "/Users/machunxiao/workspace/research/jvm/test/Main.class";
  try {
    jvm::ClassFile cf(path);

  } catch (const std::exception &e) {
    std::cout << "failed:" << e.what() << std::endl;
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
