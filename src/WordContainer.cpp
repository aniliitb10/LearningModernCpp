#include "../include/WordContainer.h"

#include <algorithm>
#include <numeric>
#include <initializer_list>
#include <exception>

void WordContainer::add(std::string str) {
  if (str.size() > max_key_width) {
    throw std::runtime_error{"Length of {" + str + "} exceeds maximum supported length (8 chars)"};
  }
  auto key = getKey(str);
  _map.emplace(key, std::move(str));
}

bool WordContainer::contains(const std::string &str) {
  auto key = getKey(str);
  return _map.find(key) != _map.end();
}

WordContainer::key_t WordContainer::getKey(const std::string &key) {
    return std::accumulate(key.cbegin(), key.cend(), 1ull,
      [this](auto product, auto right) { return product * _charMap.prime(right); });
}

/**
 * The idea is to generate all permutation of keys (using primes from prime map)
 * -- this is done recursively in this function with the help of index
 * Following is quick dry run of what this method does
 *  => when index == 0, the vector contains only zeros
 *     -- get the current_prime by keys.at(index)
 *     -- insert at 0th index (it is the first available index)
 *  => when index == 1, 0the index already has a prime.
 *     -- get the current_prime by keys.at(index)
 *     -- insert at 1st index (it is the next available index)
 *     -- multiply prime at 0th index by current_prime and store at 2nd index
 *  => when index == 2, 0th to 3rd indices are already filled
 *     -- get the current_prime by keys.at(index)
 *     -- insert at 4th index (it is the next available index)
 *     -- multiply primes from 0th to 3rd index by current_prime and store them at 5th to 6th indices respectively
 *  => when index == n, 0th to n^2-1 indices are already filled
 *     -- get the current_prime by keys.at(index)
 *     -- insert at n^2th index (it is the next available index)
 *     -- multiply primes from 0th to n^2-1 indices by current_prime and store them at n^2+1th to n^(2+1)-2 indices respectively
 *
 *  This way, we get all 2^n-1 non-empty possible combinations of n primes
 *
 * @param keys
 * @param index
 * @param permutations
 */

void WordContainer::getAllKeyPermutations(const std::vector<WordContainer::key_t> &keys, std::size_t index,
                                                          std::vector<WordContainer::key_t>& permutations) {
  if (index == keys.size()) {
    return; // done with all keys!
  }

  const auto prime = keys.at(index);

  if (index != 0) {

    auto begin = (1ull << index);
    auto end = begin * 2 - 1;

    permutations.at(begin-1) = prime;

    for (size_t i = 0; begin < end; ++i, ++begin) {
      permutations.at(begin) = permutations.at(i) * prime;
    }
  }

  else { // index == 0
    permutations.at(index) = prime;
  }

  getAllKeyPermutations(keys, index+1, permutations);
}

std::vector<WordContainer::key_t> WordContainer::getKeys(const std::string &str) {
  std::vector<WordContainer::key_t> keys{};
  std::transform(str.begin(), str.end(), std::back_inserter(keys), [this](auto c){ return _charMap.prime(c); });
  return keys;
}