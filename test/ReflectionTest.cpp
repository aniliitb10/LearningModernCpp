#include "../include/Person.h"
#include <gtest/gtest.h>

class ReflectionTest : public ::testing::Test
{
protected:
  void SetUp() override
  {
    _p1 = Person("Anil", "Kumar");
    _sameAsP1 = _p1;
    _notP1 = Person("FirstName", "SecondName");
  }

  Person _p1, _sameAsP1, _notP1;
};

TEST_F(ReflectionTest, EqualToOperatorTest)
{
  EXPECT_TRUE(_p1 == _sameAsP1);
  EXPECT_FALSE(_p1 == _notP1);

  const auto& p1ref = _p1;
  EXPECT_TRUE(_p1 == p1ref);
}

TEST_F(ReflectionTest, NotEqualToOperatorTest)
{
  EXPECT_FALSE(_p1 != _sameAsP1);
  EXPECT_TRUE(_p1 != _notP1);

  const auto& p1ref = _p1;
  EXPECT_FALSE(_p1 != p1ref);
}

TEST_F(ReflectionTest, InputOutputOpTest) {
  Person p = _p1;
  std::ostringstream os;
  os << p;
  EXPECT_EQ(os.str(), "Anil Kumar ");

  os.str("");
  os << _notP1;
  std::istringstream is(os.str());
  EXPECT_EQ(is.str(), "FirstName SecondName ");
  is >> p;
  EXPECT_EQ(p, _notP1);
}