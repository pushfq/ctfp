#pragma once

#include <algorithm>
#include <array>
#include <cstdint>
#include <span>

namespace ctfp {

constexpr char wildcard = '?';

template <size_t N>
struct string : std::array<char, N> {
  constexpr string(const char (&str)[N]) : std::array<char, N>(std::to_array(str)) {
  }
};

struct match {
  constexpr match() = default;

  constexpr match(bool is_wildcard, std::byte value) : is_wildcard_(is_wildcard), value_(value) {
  }

  constexpr bool operator==(std::byte byte) const {
    return value_ == byte || is_wildcard_;
  }

 private:
  bool is_wildcard_;
  std::byte value_;
};

constexpr bool is_digit(char ch) {
  return ch >= '0' && ch <= '9';
}

constexpr bool is_hex_digit(char ch) {
  return is_digit(ch) || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F');
}

constexpr std::byte char_to_hex(char ch) {
  if (ch >= '0' && ch <= '9')
    return static_cast<std::byte>(ch - '0');

  if (ch >= 'a' && ch <= 'f')
    return static_cast<std::byte>(ch - 'a' + 10);

  if (ch >= 'A' && ch <= 'F')
    return static_cast<std::byte>(ch - 'A' + 10);

  return std::byte(0);
}

constexpr std::byte stitch(char x, char y) {
  return (char_to_hex(x) << 4) | char_to_hex(y);
}

template <string Pattern>
consteval size_t count_bytes() {
  size_t result = 0;
  for (size_t i = 0; i < Pattern.size(); ++i) {
    if (Pattern[i] == wildcard)
      ++result;
    else if (is_hex_digit(Pattern[i]) && is_hex_digit(Pattern[i + 1]))
      ++result, ++i;
  }
  return result;
}

template <string Pattern>
using pattern_array = std::array<match, count_bytes<Pattern>()>;

template <string Pattern>
consteval pattern_array<Pattern> build() {
  pattern_array<Pattern> result;
  for (size_t i = 0, j = 0; i < Pattern.size(); ++i) {
    if (Pattern[i] == wildcard) {
      result[j++] = match(true, {});
    } else if (is_hex_digit(Pattern[i]) && is_hex_digit(Pattern[i + 1])) {
      result[j++] = match(false, stitch(Pattern[i], Pattern[i + 1]));
      ++i;
    }
  }
  return result;
}

template <string Pattern, typename T = std::uintptr_t>
T find(std::span<const std::byte> memory) {
  constexpr static auto compiled_pattern = build<Pattern>();

  const auto it = std::search(memory.begin(), memory.end(), compiled_pattern.begin(), compiled_pattern.end());

  if (it == memory.end()) {
    return T{};
  }

  return reinterpret_cast<T>(std::addressof(*it));
}

}  // namespace ctfp
