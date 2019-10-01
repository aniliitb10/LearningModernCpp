#include <gtest/gtest.h>

#include "../include/WordContainer.h"
#include <set>

struct WordContainerTest : ::testing::Test {
  WordContainer wc{};

  void SetUp() override  {
    wc = WordContainer{};
  }
};

TEST_F(WordContainerTest, SimpleTest) {
  const std::string name{"simple"};
  wc.add(name);
  EXPECT_TRUE(wc.contains(name));
}

TEST_F(WordContainerTest, GetTest) {

  wc.add("wo");
  wc.add("wom");
  wc.add("me");
  wc.add("men");
  wc.add("man");
  wc.add("woman");
  wc.add("women");

  std::set<std::string> words{};
  wc.get("women", std::inserter(words, words.end()));

  std::set<std::string> expected{"wo", "wom", "me", "men", "women"};
  EXPECT_EQ(expected, words);
}