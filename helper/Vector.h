#pragma once

#include <array>
#include <exception>

namespace my {

  template<typename T, std::size_t MaxSize = 10>
  class static_vector {

  public:

    constexpr static_vector() = default;

    constexpr void push_back(T value) {
      must_not_be_full();
      _values[_index++] = std::move(value);
    }

    constexpr const T& back() const {
      must_not_be_empty();
      return _values.at(_index-1);
    }

    constexpr const T& front() const {
      must_not_be_empty();
      return _values.at(_index-1);
    }

    constexpr const T& operator[](std::size_t index) const {
      must_be_valid_access(index);
      return _values[index];
    }

    constexpr std::size_t size() const noexcept {
      return _index;
    }

    constexpr std::size_t max_size() const noexcept {
      return _values.size();
    }

    constexpr auto begin() const noexcept {
      return _values.begin();
    }

    constexpr auto begin() noexcept {
      return _values.begin();
    }

    constexpr auto end() const noexcept {
      return std::next(_values.begin(), size());
    }

    constexpr auto end() noexcept {
      return std::next(_values.begin(), size());
    }

  private:
    constexpr void must_not_be_empty() const {
      if (_index == 0) {
        throw std::runtime_error{"Empty array"};
      }
    }

    constexpr void must_not_be_full() const {
      if (_index == _values.size()) {
        throw std::runtime_error{"Array is full"};
      }
    }

    constexpr void must_be_valid_access(std::size_t index) const {
      if (index >= this->size()) {
        throw std::runtime_error{"Invalid access"};
      }
    }

    std::size_t _index{0}; // index where next element will be placed
    std::array<T, MaxSize> _values{};
  };
}