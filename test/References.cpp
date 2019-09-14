#include <gtest/gtest.h>
#include <type_traits>

struct References : public ::testing::Test
{
  class Person
  {
  public:
    template <typename T, typename = std::enable_if_t <std::is_constructible_v<std::string, T>, T>>
    explicit Person(T&& name_):
    _name(std::forward<T>(name_)) {}

    const std::string& getName() const
    {
      return _name;
    }

    bool isRvalue()&& { return true;}
    bool isRvalue()& { return false;}

    template <typename T>
    constexpr static decltype(auto) test_forward_ref(T&& obj)
    {
      // as all parameters are always lvalues, here is a test
      using decayed_t = std::decay_t<T>;
      static_assert(std::is_same_v<decayed_t, Person>);
      EXPECT_TRUE(!obj.isRvalue());

      return std::forward<T>(obj);
    }

    /**
     * BAD FUNCTION
     * This doesn't even compile when arg is a rvalue!
     * As Person& (lvalue reference) can't bind with an rvalue
     * */
    template <typename T>
    constexpr static T&& scott_meyers_forward(std::remove_reference_t<T>& arg) {
      return static_cast<T&&>(arg);
    }

  private:
    std::string _name;
  };
};

TEST_F(References, CtrTest)
{
  std::string name{"cpp_programmer"};
  const Person person{name};
  EXPECT_EQ(person.getName(), name);

  const Person moved_person{std::move(name)};
  // UB to use the std::string after it has been move from
  // but should be okay for testing
  EXPECT_TRUE(name.empty());
  EXPECT_EQ(moved_person.getName(), "cpp_programmer");
}

TEST_F(References, DecayTest)
{
  using GetNameType = decltype(std::declval<Person>().getName());

  // std::decay removes the reference and const-ness
  static_assert(std::is_same_v<std::decay_t<GetNameType>, std::string>);

  // just removing reference is not enough
  static_assert(!std::is_same_v<std::remove_reference_t<GetNameType>, std::string>);

  // Need to remove const-ness as well
  static_assert(std::is_same_v<std::remove_const_t<std::remove_reference_t<GetNameType>>, std::string>);
}

TEST_F(References, ConstructionTest)
{
  Person person{"cpp_programmer"};

  // Ensuring that person doesn't have any reference
  static_assert(std::is_same_v<std::remove_reference_t<decltype(person)>, References::Person>);

  // Just ensuring that Constructor taking Universal Reference is not interfering with Copy Constructor
  Person copyOfPerson{person};

  // Further constructor test
  static_assert(std::is_constructible_v<Person, std::string>);

  // from string literal
  static_assert(std::is_constructible_v<Person, decltype("any_testing_string_literal")>);

  // from char arrays as well
  auto char_array = "char array"; // const char char_array[11]
  static_assert(std::is_constructible_v<Person, decltype(char_array)>);

  // even from const char*
  static_assert(std::is_same_v<const char*, std::decay_t<decltype(char_array)>>);
  static_assert(std::is_constructible_v<Person, std::decay_t<decltype(char_array)>>);

  // Even from the decayed std::string
  static_assert(std::is_constructible_v<Person, std::decay_t<std::string>>);

  // But not from anything else, like: int, etc:
  static_assert(!std::is_constructible_v<Person, int>);
  static_assert(!std::is_constructible_v<Person, bool>);
}

TEST_F(References, RemoveRefTest)
{
  // Removing references
  static_assert(std::is_same_v<std::remove_reference_t<Person>, Person>);
  static_assert(std::is_same_v<std::remove_reference_t<Person&>, Person>);
  static_assert(std::is_same_v<std::remove_reference_t<Person&&>, Person>);
}

TEST_F(References, AddingRefTest)
{
  // Adding lvalue reference
  static_assert(std::is_same_v<std::add_lvalue_reference_t<Person>, Person&>);
  static_assert(!std::is_same_v<std::add_lvalue_reference_t<Person>, Person&&>);
  static_assert(!std::is_same_v<std::add_lvalue_reference_t<Person>, Person>);

  // Adding rvalue reference
  static_assert(std::is_same_v<std::add_rvalue_reference_t<Person>, Person&&>);
  static_assert(!std::is_same_v<std::add_rvalue_reference_t<Person>, Person&>);
  static_assert(!std::is_same_v<std::add_rvalue_reference_t<Person>, Person>);
}

/**
 * This should be real fun!
 * This applies for collapsing two references
 * (hence 4 possible combination of rvalue, lvalue references):
 *  => If either of the references is an lvalue ref, then resulting reference is an lvalue ref
 *  =>      otherwise (rvalue ref with rvalue ref), it is an ravlue ref
 *
 */
TEST_F(References, ReferenceCollapsingTest)
{
  // lvalue-ref with lvalue-ref: should result in lvalue-ref
  static_assert(std::is_same_v<std::add_lvalue_reference_t<Person&>, Person&>);
  static_assert(!std::is_same_v<std::add_lvalue_reference_t<Person&>, Person&&>);
  static_assert(!std::is_same_v<std::add_lvalue_reference_t<Person&>, Person>);

  // lvalue-ref with rvalue-ref: should result in lvalue-ref
  static_assert(std::is_same_v<std::add_lvalue_reference_t<Person&&>, Person&>);
  static_assert(!std::is_same_v<std::add_lvalue_reference_t<Person&&>, Person&&>);
  static_assert(!std::is_same_v<std::add_lvalue_reference_t<Person&&>, Person>);

  // rvalue-ref with lvalue-ref: should result in lvalue-ref
  static_assert(std::is_same_v<std::add_rvalue_reference_t<Person&>, Person&>);
  static_assert(!std::is_same_v<std::add_rvalue_reference_t<Person&>, Person&&>);
  static_assert(!std::is_same_v<std::add_rvalue_reference_t<Person&>, Person>);

  // rvalue-ref with rvalue-ref: should result in rvalue-ref
  static_assert(std::is_same_v<std::add_rvalue_reference_t<Person&&>, Person&&>);
  static_assert(!std::is_same_v<std::add_rvalue_reference_t<Person&&>, Person&>);
  static_assert(!std::is_same_v<std::add_rvalue_reference_t<Person&&>, Person>);
}

TEST_F(References, MoveTest)
{
  Person person{"cpp_programmer"};

  // std::move on non-const lvalue returns rvalue ref to the non-const type
  static_assert(std::is_same_v<decltype(std::move(person)), Person&&>);
  static_assert(!std::is_same_v<decltype(std::move(person)), Person&>);
  static_assert(!std::is_same_v<decltype(std::move(person)), Person>);
  static_assert(!std::is_same_v<decltype(std::move(person)), const Person&&>);

  // std::move on const lvalue returns rvalue-ref to const
  const Person const_person{person};
  static_assert(std::is_same_v<decltype(std::move(const_person)), const Person&&>);
  static_assert(!std::is_same_v<decltype(std::move(const_person)), Person&&>);
  static_assert(!std::is_same_v<decltype(std::move(const_person)), Person&>);
  static_assert(!std::is_same_v<decltype(std::move(const_person)), Person>);
}

/**
 * It's tricky here!
 * for function like:
 *
 * decltype(auto) f(T&& arg) {
 *    return std::forward<T>(arg);
 * }
 *
 * if arg is an lvalue, T is deduced to be lvalue reference
 * hence, if outside such functions, if we try to pass arg manually
 * then calling std::forward<T&>(arg) should be the correct way to do so
 * instead of   std::forward<T>(arg)
 *
 * In a nutshell: calling std::forward without involving type deduction
 * is nothing more than static_cast-ing
 * */

TEST_F(References, StdForwardTest)
{
  Person person{"cpp_programmer"};

  // when template type is Person, then return type is Person&&, as std::forward casts into T&&
  // and T here is Person&&, so reference collapsing makes it again Person&&
  // this is never the case when type deduction happens
  // in fact, this is what happens std::move() is called on lvalues
  static_assert(std::is_same_v<decltype(std::forward<Person>(person)), Person&&>);
  static_assert(std::is_same_v<decltype(std::forward<Person>(person)), decltype(std::move(person))>);
  // By definition, rvalue reference returned by a function is an rvalue
  EXPECT_TRUE(std::forward<Person>(Person("common_name")).isRvalue());

  // std::forward on lvalue returns lvalue reference to the lvalue
  // std::forward's && on Person& is collapsed to just Person&
  static_assert(std::is_same_v<decltype(std::forward<Person&>(person)), Person&>);
  // By definition, lvalue references are lvalues
  EXPECT_FALSE(std::forward<Person&>(person).isRvalue());

  // CHEATING: argument type is lvalue but the template type is rvalue reference
  // this works as an lvalue can be casted into rvalue reference
  // this is what std::move does with lvalues
  // this is never the case when type detection happens
  static_assert(std::is_same_v<decltype(std::forward<Person&&>(person)), Person&&>);
  static_assert(std::is_same_v<decltype(std::forward<Person&&>(person)), decltype(std::move(person))>);

  // std::forward on rvalue returns rvalue reference (same as when type deduction heppens)
  static_assert(std::is_same_v<decltype(std::forward<Person&&>(Person{"some_name"})), Person&&>);
  // By definition, rvalue references returned from a function are rvalues
  EXPECT_TRUE(std::forward<Person&&>(Person("common_name")).isRvalue());
  // for rvalues, effect is same as calling std::move as well
  static_assert(std::is_same_v<decltype(std::forward<Person&&>(Person{"some_name"})), decltype(std::move(Person{"some_name"}))>);

  // This is IMPRESSIVE: rvalue reference overload has an internal check (static_assert) that
  // type (lvalue reference here) is not an lvalue reference
  // as casting an rvalue to lvalue reference should be UB
  // hence, following fails to compile
  // static_assert(std::is_same_v<decltype(std::forward<Person&>(Person{"some_name"})), Person&&>);
}

/**
 * To familiarize with the behavior of std::declval
 * It actually adds an rvalue ref to the type
 */
TEST_F(References, DeclValTest)
{
  Person person{"dummy person"};
  static_assert(std::is_same_v<decltype(person), Person>);

  // special case, mentioned in Effective Modern C++, (person) yields an expression more complicated type than a name
  static_assert(std::is_same_v<decltype((person)), Person&>);

  // Simply changes from non-reference type Person to rvalue ref type Person
  static_assert(std::is_same_v<decltype(std::declval<Person>()), Person&&>);

  // rvalue ref over rvalue ref => rvalue ref (reference collapsing rule)
  static_assert(std::is_same_v<decltype(std::declval<Person&&>()), Person&&>);

  // rvalue ref over lvalue ref => lvalue ref (reference collapsing rule)
  static_assert(std::is_same_v<decltype(std::declval<Person&>()), Person&>);

  // lvalue ref over rvalue ref (generated from rvalue ref with rvalue ref) => lvalue ref (reference collapsing rule)
  static_assert(std::is_same_v<std::add_lvalue_reference_t<decltype(std::declval<Person&&>())>, Person&>);

  // lvalue ref over lvalue ref (generated from rvalue ref with lvalue ref) => lvalue ref (reference collapsing rule)
  static_assert(std::is_same_v<std::add_lvalue_reference_t<decltype(std::declval<Person&>())>, Person&>);
}

/**
 * std::declval has been used to get the return types
 * It simply adds rvalue_reference to type T
 * and hence, using reference collapsing rules, it can be used to get lvalue reference as well
 */
TEST_F(References, ForwardRefTest)
{
  // If an lvalue reference is passed to test_forward_ref, then returned type is also an lvalue reference
  static_assert(std::is_same_v<decltype(Person::test_forward_ref(std::declval<Person&>())), Person&>);
  static_assert(!std::is_same_v<decltype(Person::test_forward_ref(std::declval<Person&>())), Person>);
  static_assert(!std::is_same_v<decltype(Person::test_forward_ref(std::declval<Person&>())), Person&&>);

  // If an rvalue reference is passed to test_forward_ref, then returned type is rvalue reference
  static_assert(std::is_same_v<decltype(Person::test_forward_ref(std::declval<Person>())), Person&&>);
  static_assert(!std::is_same_v<decltype(Person::test_forward_ref(std::declval<Person>())), Person&>);
  static_assert(!std::is_same_v<decltype(Person::test_forward_ref(std::declval<Person>())), Person>);

  // By definition, rvalue reference returned by
  // a function is an rvalue
  EXPECT_EQ(true, Person::test_forward_ref(Person("common_name")).isRvalue());

  // By definition, lvalue references are lvalues
  Person person{"common_name_again"};
  EXPECT_EQ(false, Person::test_forward_ref(person).isRvalue());
}

/**
 Scott Meyers's version doesn't compile with rvalues
 otherwise, it works well with lvalues
*/
TEST_F(References, ScottMeyersForwardTest)
{
  Person person{"common_name"};
  static_assert(std::is_same_v<decltype(std::forward<Person&>(person)),
                               decltype(Person::scott_meyers_forward<Person&>(person))>);

  // CHEATING! for Scott's version, I am passing an lvalue but template type is rvalue reference
  static_assert(std::is_same_v<decltype(std::forward<Person&&>(Person{"common_name"})),
                               decltype(Person::scott_meyers_forward<Person&&>(person))>);

  // compilation error! As Person& (lvalue reference) can't bind to an rvalue
  //  static_assert(std::is_same_v<decltype(std::forward<Person&&>(Person{"common_name"})),
  //                               decltype(Person::scott_meyers_forward<Person&&>(Person{"common_name"}))>);
}