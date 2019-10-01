#pragma once

#include "CharPrimeMap.h"

#include <unordered_map>
#include <string>
#include <vector>

class WordContainer {
public:
  using key_t = uint64_t;
  using storage_t = std::unordered_map<key_t, std::string>;

  /**
   * Adds to the existing collection, doesn't handle anagrams yet
   * Maximum supported @str length is 8
   * @param str
   */
  void add(std::string str);

  template <typename OutItr>
  void get(const std::string &str, OutItr outItr);

  bool contains(const std::string& str);

private:

  // last prime in char-prime map is 239 for ('z') and 239^9 exceeds std::numeric_limits<uint64_t>::max();
  // if query string is more than 8 chars long, then we won't be able to handle this properly
  // as for n char long string, largest prime product will involve multiplication of prime of
  // all chars. In case of: "zzzzzzzzz" ('z' repeated 9 times, prime product will exceed
  // std::numeric_limits<uint64_t>::max();
  static constexpr key_t max_key_width{8};

  /**
   * @permutations must already be big enough to store all permutations of keys
   */
  static void getAllKeyPermutations(const std::vector<key_t> &keys, std::size_t index, std::vector<key_t>& permutations);

  key_t getKey(const std::string& key);

  std::vector<key_t> getKeys(const std::string &str);

  storage_t _map;
  my::alphabet_char_prime_map _charMap{};
};

template<typename OutItr>
void WordContainer::get(const std::string &str, OutItr outItr) {
  const auto& queryStr = (str.size() > max_key_width) ? str.substr(0, max_key_width) : str;
  auto keys = getKeys(queryStr);
  auto all_permutation_size = (1ull << queryStr.size()) - 1;
  std::vector<key_t> all_permutation(all_permutation_size);
  getAllKeyPermutations(getKeys(queryStr), 0, all_permutation);

  for (auto key : all_permutation) {
    auto itr = _map.find(key);
    if (itr != _map.end()) {
      outItr = itr->second;
    }
  }
}