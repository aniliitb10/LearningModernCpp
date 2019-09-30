#pragma once

#include "TupleUtil.h"
#include <ostream>
#include <iostream>

/**
 * The idea is to test whether type T has reflect() member or not
 * if it has, use reflect() member to implement operators ==, != and <<
 * std::experimental::is_detected doesn't make sense here because if type T
 * doesn't implement reflect(), then std::enable_if_t<IsReflectable, bool> is anyway going
 * fail template substitution
 * std::void_t suits better!

Instead of:

template <typename T, bool IsReflectable = exp::is_detected_v<reflect_member_func, T>>
auto operator== (const T& left_, const T& right_) -> std::enable_if_t<IsReflectable, bool>
{
  return left_.reflect() == right_.reflect();
}

this should do the job!

template <typename T, typename = std::void_t<reflect_member_func<T>>>
auto operator== (const T& left_, const T& right_) -> bool
{
  return left_.reflect() == right_.reflect();
}

 */

template <typename T>
using reflect_member_func = decltype(&T::reflect);

template <typename T>
using mut_reflect_member_func = decltype(&T::mut_reflect);


template <typename T, typename = std::void_t<reflect_member_func<T>>>
auto operator== (const T& left_, const T& right_) -> bool
{
  return left_.reflect() == right_.reflect();
}

template <typename T, typename = std::void_t<reflect_member_func <T>>>
auto operator!= (const T& left_, const T& right_) -> bool
{
  return left_.reflect() != right_.reflect();
}

template <typename T, typename = std::void_t <reflect_member_func <T>>>
auto operator<< (std::ostream& os_, const T& obj) ->  std::ostream&
{
  tuple_for_each(obj.reflect(), [&os_](const auto &elem_) { os_ << elem_ << " "; });
  return os_;
}

template <typename T, typename = std::void_t <mut_reflect_member_func<T>>>
auto operator>> (std::istream& is, T& obj) ->  std::istream&
{
  tuple_for_each(obj.mut_reflect(), [&is](auto &elem_) { is >> elem_; });
  return is;
}