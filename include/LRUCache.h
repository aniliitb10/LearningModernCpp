#pragma once

#include <unordered_map>
#include <list>

/**
 * The idea is to use a linked list and a map
 * to implement least-recently-used-cache (LRUCache).
 * Map will act like a cache and linkedList
 * will help finding the least-recently-used-element
 */

template<typename Key, typename Value>
class LRUCache {
public:

  // aliases
  using value_type = std::pair<const Key, Value>;
  using list_type = std::list<value_type>;
  using iterator = typename list_type::iterator;
  using const_iterator = typename list_type::const_iterator;
  using cache_type = std::unordered_map<Key, iterator>;

  explicit LRUCache(std::size_t size_) : _maxSize(size_) {}

  std::size_t size() { return _cache.size(); }

  std::size_t maxSize() { return _maxSize; }

  // if the key exists, it is made most recently used key
  iterator get(const Key &key) {
    auto cacheItr = _cache.find(key);

    if (cacheItr != _cache.end()) {
      // just move corresponding listItr to the end
      _accessList.splice(_accessList.end(), _accessList, cacheItr->second);
      return lastItr();
    }

    return _accessList.end();
  }

  // if the key exists, it is made most recently used key
  // without overwriting the key
  // otherwise, new key-value pair inserted and made most recently used key
  iterator insert(const Key &key, const Value &value) {
    auto cacheItr = _cache.find(key);

    if (cacheItr == _cache.end()) {
      _accessList.emplace_back(key, value);
      _cache.emplace(key, lastItr());

      if (_cache.size() > _maxSize) {
        auto firstItr = _accessList.begin();
        _cache.erase(firstItr->first);
        _accessList.pop_front();
      }


    } else {
      // just move corresponding listItr to end
      _accessList.splice(_accessList.end(), _accessList, cacheItr->second);
    }


    return lastItr();
  }

  void erase(const Key& key) {
    auto cacheItr = _cache.find(key);
    if (cacheItr == _cache.end()) {
      return;
    }

    _accessList.erase(cacheItr->second);
    _cache.erase(cacheItr);
  }

  // this doesn't alter the access order
  iterator begin() {
    return _accessList.begin();
  }

  // this doesn't alter the access order
  iterator end() {
    return _accessList.end();
  }

private:

  iterator lastItr() {
    return std::prev(_accessList.end());
  }

  std::size_t _maxSize;
  cache_type _cache{};
  list_type _accessList{};
};