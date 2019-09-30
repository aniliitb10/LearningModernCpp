#include <gtest/gtest.h>

#include "../helper/CharPrimeMap.h"
#include "../helper/algorithm.h"

#include <algorithm>

struct CharPrimeMapTest : ::testing::Test {
  static constexpr my::alphabet_char_prime_map map{};

  constexpr static bool brute_force_is_prime(std::size_t num) {
    if (num == 2 || num == 3) {
      return true;
    }

    for (std::size_t n = 5; n*n <= num; n += 2) {
      if (num % n == 0) {
        return false;
      }
    }

    return true;
  }
};

TEST_F(CharPrimeMapTest, CompileTimeComputationTest) {
  static_assert(map.prime('A') == 2);
  static_assert(map.prime('B') == 3);
  static_assert(map.prime('C') == 5);
  static_assert(map.prime('D') == 7);
  static_assert(map.prime('E') == 11);
  static_assert(map.prime('F') == 13);
  static_assert(map.prime('G') == 17);
  static_assert(map.prime('H') == 19);
  static_assert(map.prime('I') == 23);
  static_assert(map.prime('J') == 29);
  static_assert(map.prime('K') == 31);
  static_assert(map.prime('L') == 37);
  static_assert(map.prime('M') == 41);
  static_assert(map.prime('N') == 43);
  static_assert(map.prime('O') == 47);
  static_assert(map.prime('P') == 53);
  static_assert(map.prime('Q') == 59);
  static_assert(map.prime('R') == 61);
  static_assert(map.prime('S') == 67);
  static_assert(map.prime('T') == 71);
  static_assert(map.prime('U') == 73);
  static_assert(map.prime('V') == 79);
  static_assert(map.prime('W') == 83);
  static_assert(map.prime('X') == 89);
  static_assert(map.prime('Y') == 97);
  static_assert(map.prime('Z') == 101);
  static_assert(map.prime('a') == 103);
  static_assert(map.prime('b') == 107);
  static_assert(map.prime('c') == 109);
  static_assert(map.prime('d') == 113);
  static_assert(map.prime('e') == 127);
  static_assert(map.prime('f') == 131);
  static_assert(map.prime('g') == 137);
  static_assert(map.prime('h') == 139);
  static_assert(map.prime('i') == 149);
  static_assert(map.prime('j') == 151);
  static_assert(map.prime('k') == 157);
  static_assert(map.prime('l') == 163);
  static_assert(map.prime('m') == 167);
  static_assert(map.prime('n') == 173);
  static_assert(map.prime('o') == 179);
  static_assert(map.prime('p') == 181);
  static_assert(map.prime('q') == 191);
  static_assert(map.prime('r') == 193);
  static_assert(map.prime('s') == 197);
  static_assert(map.prime('t') == 199);
  static_assert(map.prime('u') == 211);
  static_assert(map.prime('v') == 223);
  static_assert(map.prime('w') == 227);
  static_assert(map.prime('x') == 229);
  static_assert(map.prime('y') == 233);
  static_assert(map.prime('z') == 239);
}

TEST_F(CharPrimeMapTest, PrimeValueCheck) {
  std::array<char, 52> alphabet_chars{};
  my::iota(alphabet_chars.begin(), std::next(alphabet_chars.begin(), 26), 'A');
  my::iota(std::next(alphabet_chars.begin(), 26), alphabet_chars.end(), 'a');

  for (auto c : alphabet_chars) {
    EXPECT_TRUE(brute_force_is_prime(map.prime(c)));
  }
}