//
// Created by machunxiao on 2020/4/8.
//
#include "Common.h"
#include <codecvt>
#include <locale>

namespace jvm {

// https://en.cppreference.com/w/cpp/locale/codecvt_utf8

// https://codereview.stackexchange.com/questions/419/converting-between-stdwstring-and-stdstring

std::wstring Utf8ToWstring(const std::string &str) {
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
  return convert.from_bytes(str);
}
std::string WstringToUtf8(const std::wstring &str) {
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> convert;
  return convert.to_bytes(str);
}

} // namespace jvm