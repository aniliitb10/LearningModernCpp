#include <gtest/gtest.h>
#include <string>
#include <random>

#include "../LRUCache/LRUCache.h"

struct LRUCacheTest : public ::testing::Test {

  void SetUp() override {
    _lruCache = LRUCache<int, std::string>{5};
  }

  int getRandomNum() {
    return dist(_engine);
  }

  constexpr static std::size_t _maxSize{5};
  LRUCache<int, std::string> _lruCache{_maxSize};

  std::random_device _dev{};
  std::mt19937 _engine{_dev()};
  std::uniform_int_distribution<typename decltype(_engine)::result_type> dist{0, _maxSize-1};
};

TEST_F(LRUCacheTest, FixedSizeTest) {
  const auto maxSize = _lruCache.maxSize();
  for (int i = 0; i < maxSize; ++i) {
    EXPECT_EQ(i, _lruCache.size());
    _lruCache.insert(i, std::to_string(i));
  }

  for (int i = maxSize; i < 1000; ++i) {
    _lruCache.insert(i, std::to_string(i));
    EXPECT_EQ(maxSize, _lruCache.size());
  }
}

TEST_F(LRUCacheTest, InsertionOrderTest) {

  const auto maxSize = _lruCache.maxSize();
  for (int i = 0; i < maxSize; ++i) {
    _lruCache.insert(i, std::to_string(i));
  }

  // As now all are being just inserted
  // so after each insertion, the cached data
  // should only be of the last 5 insertions
  for (int i = maxSize; i < 1000; ++i) {
    _lruCache.insert(i, std::to_string(i));

    auto itr = _lruCache.begin();
    for (int j = i-maxSize+1; j <= i; ++j, std::advance(itr, 1)) {
      EXPECT_EQ(j, itr->first);
      EXPECT_EQ(std::to_string(j), itr->second);
    }
  }
}

TEST_F(LRUCacheTest, LatestAccessTest) {

  for (int i = 0; i < _lruCache.maxSize(); ++i) {
    _lruCache.insert(i, std::to_string(i));
  }

  for (int i = 0; i < 1000; ++i) {
    int randNum = getRandomNum();
    _lruCache.get(randNum);
    EXPECT_EQ(randNum, std::prev(_lruCache.end())->first);
  }
}