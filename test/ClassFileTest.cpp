//
// Created by machunxiao on 2020/4/8.
//
#include "ClassFile.h"
#include "TestConfig.h"
#include <gtest/gtest.h>
#include <iostream>

TEST(class_file, parse) {
  std::string s;
  s.append(PATH_PREFIX);
  s.append("/Main.class");
  const char *path = s.c_str();
  try {
    jvm::ClassFile cf(path);
    std::cout << cf.ToString() << std::endl;
  } catch (const std::exception &e) {
    std::cout << "failed:" << e.what() << std::endl;
  }
}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
