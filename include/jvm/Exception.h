//
// Created by machunxiao on 2020/4/7.
//

#ifndef MINI_JVM_EXCEPTION_H
#define MINI_JVM_EXCEPTION_H
#include "Common.h"
#include <exception>
namespace jvm {

// region ConstantPoolException

class ConstantPoolException : public std::exception {

public:
  explicit ConstantPoolException(u4 index_) : index(index_) {}
  [[nodiscard]] const char *what() const noexcept override { return ""; }

public:
  u4 index;
};

class InvalidEntry : public ConstantPoolException {

public:
  InvalidEntry(u4 index_, u1 tag_) : ConstantPoolException(index_) {
    tag = tag_;
  }
  [[nodiscard]] const char *what() const noexcept override {
    std::string msg;
    msg.append("unexpected tag at #");
    msg.append(std::to_string(index));
    msg.append(": ");
    msg.append(std::to_string(tag));
    return msg.c_str();
  }

public:
  u1 tag;
};

class InvalidIndex : public ConstantPoolException {

public:
  explicit InvalidIndex(u4 index_) : ConstantPoolException(index_) {}
  [[nodiscard]] const char *what() const noexcept override {
    std::string msg;
    msg.append("invalid index #");
    msg.append(std::to_string(index));
    return msg.c_str();
  }
};

class UnexpectedEntry : public ConstantPoolException {

public:
  UnexpectedEntry(u4 index_, u1 expected_tag_, u1 found_tag_)
      : ConstantPoolException(index_) {
    expected_tag = expected_tag_;
    found_tag = found_tag_;
  }
  [[nodiscard]] const char *what() const noexcept override {
    std::string msg;
    msg.append("unexpected entry at #");
    msg.append(" -- expected tag ");
    msg.append(std::to_string(expected_tag));
    msg.append(", found ");
    msg.append(std::to_string(found_tag));
    return msg.c_str();
  }

public:
  u1 expected_tag;
  u1 found_tag;
};

class EntryNotFound : public ConstantPoolException {

public:
  explicit EntryNotFound(const std::string &value_)
      : ConstantPoolException(-1) {
    value = value_;
  }
  [[nodiscard]] const char *what() const noexcept override {
    std::string msg;
    msg.append("value not found: ");
    msg.append(value);
    return msg.c_str();
  }

public:
  std::string value;
};

// endregion ConstantPoolException

// region AttrException

class AttrException : public std::exception {};

class InvalidAnnotation : public AttrException {

public:
  InvalidAnnotation() { msg = ""; }
  explicit InvalidAnnotation(const char *msg_) { msg = msg_; }
  explicit InvalidAnnotation(const std::string &msg_) { msg = msg_.c_str(); }
  [[nodiscard]] const char *what() const noexcept override { return msg; }

private:
  const char *msg;
};

// endregion AttrException

// region IOException

class IOException : public std::exception {
public:
  IOException() { msg = ""; }
  IOException(const char *msg_) { msg = msg_; }
  IOException(const std::string &msg_) { msg = msg_.c_str(); }
  [[nodiscard]] const char *what() const noexcept override { return msg; }

private:
  const char *msg;
};

class EofException : public IOException {
public:
  EofException() : IOException() {}
  EofException(const char *msg_) : IOException(msg_) {}
};

// endregion IOException

// region RuntimeException

class IllegalArgumentException : public std::exception {
public:
  IllegalArgumentException() { msg = ""; }
  IllegalArgumentException(const char *msg_) { msg = msg_; }
  explicit IllegalArgumentException(const std::string &msg_) {
    msg = msg_.c_str();
  }
  [[nodiscard]] const char *what() const noexcept override { return msg; }

private:
  const char *msg;
};

// endregion RuntimeException

} // namespace jvm
#endif // MINI_JVM_EXCEPTION_H
