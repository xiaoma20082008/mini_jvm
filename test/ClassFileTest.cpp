//
// Created by machunxiao on 2020/4/8.
//
#include "classfile/ClassFile.h"
#include "TestConfig.h"
#include "classfile//ClassLoader.h"
#include <gtest/gtest.h>
#include <iostream>
using namespace std;

TEST(class_file, class_loader) {
  jvm::ClassLoader loader(PATH_PREFIX);
  auto clazz = loader.LoadClass("Main");
  cout << clazz << endl;
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
