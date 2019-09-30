#include "Vector.h"
#include "algorithm.h"


namespace my {
  class alphabet_char_prime_map {

  public:

    constexpr alphabet_char_prime_map() {
      fill_primes();
    }

    constexpr std::size_t prime(char c) const {
      return _values[index(c)];
    }

  private:

    constexpr std::size_t index(char c) const {
      must_be_valid_alphabet_char(c);
      if (c >= lc_first) {
        return c - lc_first + 26; // to cover for 'A' to 'Z'
      }

      return c-uc_first;
    }

    constexpr void must_be_valid_alphabet_char(char c) const {
      if (c < uc_first || (c > uc_last && c < lc_first) || c > lc_last) {
        throw std::runtime_error{"Invalid alphabet character"};
      }
    }

    constexpr bool is_prime(size_t num) const noexcept {
      auto itr = my::find_if(_values.begin(), _values.end(), [num](auto prime){ return (num % prime) == 0; });
      return itr == _values.end();
    }

    constexpr void fill_primes() {

      // pushing few initial values
      _values.push_back(2);
      _values.push_back(3);
      _values.push_back(5);
      _values.push_back(7);
      _values.push_back(11);
      _values.push_back(13);
      _values.push_back(17);
      std::size_t last_prime{_values.back()};

      while(_values.size() != _maxSize) {
        for (std::size_t number = last_prime+2; ; number += 2) {
          if (is_prime(number)) {
            last_prime = number;
            _values.push_back(number);
            break;
          }
        }
      }
    }

    static constexpr char lc_first{'a'};
    static constexpr char lc_last{'z'};
    static constexpr char uc_first{'A'};
    static constexpr char uc_last{'Z'};

    static constexpr size_t _maxSize{52};
    static_vector<size_t, _maxSize> _values{};
  };
}