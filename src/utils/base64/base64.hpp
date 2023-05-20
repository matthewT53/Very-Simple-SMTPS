#pragma once

#include <cstdint>
#include <string>
#include <vector>

namespace smtp {

using byte = uint8_t;

class Base64 {
public:
  static const std::string Base64Encode(const std::vector<uint8_t> &data);
  static const std::vector<uint8_t> Base64Decode(const std::string &data);

  static const std::string Base64UrlEncode(const std::vector<uint8_t> &data,
                                           bool keep_padding = false);
  static const std::vector<uint8_t> Base64UrlDecode(const std::string &data);
};

} // namespace smtp
