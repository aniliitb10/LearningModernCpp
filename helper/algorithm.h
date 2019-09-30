#pragma once

/** This contains constexpr version of some algorithms from algorithm header */

namespace my {

  template <typename Itr, typename Value>
  constexpr inline void iota(Itr begin, Itr end, Value initial_value) {
    while (begin != end) {
      *begin = initial_value;
      begin++;
      initial_value++;
    }
  }

  //  as std::find is not constexpr
  template <typename Itr, typename Func>
  constexpr inline Itr find_if(Itr begin, Itr end, const Func& func) {
    while (begin != end) {
      if (func(*begin)) {
        return begin;
      }
      begin++;
    }

    return end;
  }
}