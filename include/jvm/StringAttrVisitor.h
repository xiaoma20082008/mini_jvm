//
// Created by machunxiao on 2020/4/8.
//

#ifndef MINI_JVM_STRINGATTRVISITOR_H
#define MINI_JVM_STRINGATTRVISITOR_H
#include "AttributeVisitor.h"
#include "string"
namespace jvm {
class StringAttrVisitor : public AttributeVisitor<std::string, int> {};
} // namespace jvm
#endif // MINI_JVM_STRINGATTRVISITOR_H
