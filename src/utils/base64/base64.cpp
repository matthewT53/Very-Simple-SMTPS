#include <algorithm>
#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>

#include "base64.hpp"

namespace smtp {

static const std::string b64Table =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const std::string b64UrlTable =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

static std::string base64Encode(const std::string &table, const std::vector<smtp::byte> &data);
static std::string base64EncodeBlock(const std::string &table,
                                     const std::vector<smtp::byte> &data_block, int padding = 0);

static std::vector<smtp::byte> base64Decode(const std::string &table, const std::string &data);
static std::vector<smtp::byte> base64DecodeBlock(const std::string &table,
                                                 const std::string &data_block, int padding = 0);

static smtp::byte getByteValue(const std::string &table, char value);

std::string smtp::Base64::Base64Encode(const std::string &data) {
  const std::vector<smtp::byte> data_bytes(data.begin(), data.end());
  return Base64Encode(data_bytes);
}

std::string smtp::Base64::Base64Encode(const std::vector<smtp::byte> &data) {
  return base64Encode(b64Table, data);
}

std::vector<smtp::byte> Base64::Base64Decode(const std::string &data) {
  return base64Decode(b64Table, data);
}

std::string Base64::Base64UrlEncode(const std::vector<smtp::byte> &data, bool keep_padding) {
  std::string result = base64Encode(b64UrlTable, data);
  if (!keep_padding) {
    result.erase(std::remove(result.begin(), result.end(), '='), result.end());
  }
  return result;
}

std::vector<smtp::byte> Base64::Base64UrlDecode(const std::string &data) {
  return base64Decode(b64UrlTable, data);
}

static std::string base64Encode(const std::string &table, const std::vector<smtp::byte> &data) {
  std::string result;

  int i = 0;
  int n = static_cast<int>(data.size());

  std::vector<smtp::byte> arr(3, 0x0);

  for (smtp::byte b : data) {
    arr[i++ % 3] = b;

    if (i % 3 == 0 || i == n) {
      result += base64EncodeBlock(table, arr, i % 3);
      std::fill(arr.begin(), arr.end(), 0x0);
    }
  }

  return result;
}

// Base64 encodes a block of plaintext.
// It is assumed that data_block contains a maximum of 3 smtp::bytes in size.
static std::string base64EncodeBlock(const std::string &table,
                                     const std::vector<smtp::byte> &data_block, int padding) {
  int i1 = (data_block[0] & 0xfc) >> 2;
  int i2 = ((data_block[0] & 0x3) << 4) | ((data_block[1] & 0xf0) >> 4);
  int i3 = ((data_block[1] & 0xf) << 2) | ((data_block[2] & 0xc0) >> 6);
  int i4 = (data_block[2] & 0x3f);

  std::string s(4, 0x00);
  s[0] = table[i1];
  s[1] = table[i2];
  s[2] = (padding == 1) ? '=' : table[i3];
  s[3] = (padding >= 1) ? '=' : table[i4];
  return s;
}

static std::vector<smtp::byte> base64Decode(const std::string &table, const std::string &data) {
  std::vector<smtp::byte> result;

  int i = 0;
  int n = static_cast<int>(data.length());
  std::string arr(4, 0x0);

  for (smtp::byte b : data) {
    arr[i++ % 4] = b;

    if (i % 4 == 0 || i == n) {
      const std::vector<smtp::byte> decoded = base64DecodeBlock(table, arr, i % 4);
      result.insert(result.end(), decoded.begin(), decoded.end());
    }
  }

  return result;
}

static smtp::byte getByteValue(const std::string &table, char value) {
  return value != '=' ? table.find(value) : 0;
}

// This function base64 decodes a block of 4 base64 encoded characters.
// It is assumed that data_block is 4 smtp::bytes in size.
static std::vector<smtp::byte> base64DecodeBlock(const std::string &table,
                                                 const std::string &data_block, int padding) {
  std::vector<smtp::byte> result(3);

  smtp::byte i1 = getByteValue(table, data_block[0]);
  smtp::byte i2 = getByteValue(table, data_block[1]);
  smtp::byte i3 = getByteValue(table, data_block[2]);
  smtp::byte i4 = getByteValue(table, data_block[3]);

  result[0] = ((i1 & 0x3f) << 2) | ((i2 & 0x30) >> 4);
  result[1] = ((i2 & 0xf) << 4) | ((i3 & 0x3c) >> 2);
  result[2] = ((i3 & 0x3) << 6) | i4;

  size_t padding_pos = data_block.find_first_of("=");
  int n_chars = (padding_pos == std::string::npos) ? padding : static_cast<int>(padding_pos);

  // Padding will either be 2 or 3, since it is not possible to have 1 smtp::byte
  if (n_chars > 0) {
    int num_remove = 4 - n_chars;
    result.resize(3 - num_remove);
  }

  return result;
}

} // namespace smtp
