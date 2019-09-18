#pragma once

#include "../reflection/Reflection.h"

#include <string>
#include <tuple>

/**
 * This is to help in getting reflection
 */
class Person
{
public:

  Person() = default;

  /**
   * Need both FT and ST types as if char arrays are used, they might have different types:
   * e.g. in Person{"anil", "kumar"}, FT is: char const (&)[5] while ST is: char const (&)[6]
   * */
  template <typename FT, typename ST, typename = std::enable_if_t<std::is_constructible_v<std::string, FT> &&
    std::is_constructible_v<std::string, ST>>>

  explicit Person(FT&& fName_, ST&& sName_):
  _fName(std::forward<FT>(fName_)),
  _sName(std::forward<ST>(sName_)){}

  auto reflect() const { return std::tie(_fName, _sName); }
  auto mut_reflect() { return std::tie(_fName, _sName); }

private:
  std::string _fName;
  std::string _sName;
};