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

  explicit Person(std::string fName_,std::string sName_):
  _fName(std::move(fName_)),
  _sName(std::move(sName_)){}

  auto reflect() const { return std::tie(_fName, _sName);}

private:
  std::string _fName;
  std::string _sName;
};