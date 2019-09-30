#pragma once

#include <tuple>

template <typename Tuple, typename Functor, size_t Index = 0>
auto tuple_for_each(const Tuple& tuple_, const Functor &functor_) -> void
{
  // purely for academic purpose: to verify that tuple is a reference to a const Tuple object
  static_assert(!std::is_const_v<decltype(tuple_)>);
  static_assert(std::is_const_v<std::remove_reference_t<decltype(tuple_)>>);

  constexpr auto tupleSize = std::tuple_size_v<Tuple>;
  if constexpr (Index < tupleSize)
  {
    // purely for academic purpose: to verify that each member is indeed an lvalue reference
    static_assert(std::is_lvalue_reference_v<decltype(std::get<Index>(tuple_))>);

    // constant tuple doesn't stop its members from getting modified
    // as their members are all lvalue references (references are inherently constant)
    // and the const-ness of tuple doesn't propagate to its referenced variables
    auto& value = std::get<Index>(tuple_);
    functor_(value);

    tuple_for_each<Tuple, Functor, Index + 1>(tuple_, functor_);
  }
}