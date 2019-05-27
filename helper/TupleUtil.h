#pragma once

#include <tuple>

template <typename Tuple, typename Functor, size_t Index>
auto applyFunc(const Tuple& tuple_, const Functor& functor_) -> void
{
  const auto& value = std::get<Index>(tuple_);
  functor_(value);
}

template <typename Tuple, typename Functor, size_t Index = 0>
auto tupleForEach(const Tuple& tuple_, const Functor& functor_) -> void
{
  constexpr auto tupleSize = std::tuple_size_v<Tuple>;
  if constexpr (Index < tupleSize)
  {
    applyFunc<Tuple, Functor, Index>(tuple_, functor_);
    tupleForEach<Tuple, Functor, Index + 1>(tuple_, functor_);
  }
}