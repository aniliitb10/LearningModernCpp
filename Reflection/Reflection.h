#pragma once

#include "../Helper/TupleUtil.h"
#include <ostream>

#include <experimental/type_traits>

namespace exp = std::experimental;

template <typename T>
using reflect_member_func = decltype(&T::reflect);

template <typename T, bool IsReflectable = exp::is_detected_v<reflect_member_func, T>>
auto operator== (const T& left_, const T& right_) -> std::enable_if_t<IsReflectable, bool>
{
  return left_.reflect() == right_.reflect();
}

template <typename T, bool IsReflectable = exp::is_detected_v<reflect_member_func, T>>
auto operator!= (const T& left_, const T& right_) -> std::enable_if_t<IsReflectable, bool>
{
  return left_.reflect() != right_.reflect();
}

template <typename T, bool IsReflectable = exp::is_detected_v<reflect_member_func, T>>
auto operator<< (std::ostream& os_, const T& obj) -> std::enable_if_t <IsReflectable, std::ostream&>
{
  tupleForEach(obj.reflect(), [&os_](const auto& elem_) { os_ << elem_ << " ";});
  return os_;
}