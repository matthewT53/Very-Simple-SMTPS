#include <cstring>
#include <iostream>

#include "doctest/doctest.h"

#include "utils/secure_strings.hpp"

TEST_SUITE("Secure strings") {
  TEST_CASE("Clears sensitive string buffer on destruction") {
    const std::string &expected = "this is a password";
    smtp::secure_string *password = new smtp::secure_string(expected);

    void *ptr = password->data();
    CHECK_TRUE(std::memcmp(password->data(), expected.data(), expected.size()) == 0);

    int len = static_cast<int>(expected.size());
    char arr[len] = {0x0};
    std::memset(arr, 0x0, len);

    delete password;

    // Ensure that each byte is different at this memory location and that the
    // data here does not form the original string. This is the best we can do
    // because once the memory is deallocated, the memory address may have been
    // allocated to something else.
    auto memory_check = [&]() -> bool {
      bool result = true;
      char *mem = static_cast<char *>(ptr);

      for (std::size_t i = 0; i < expected.length(); i++) {
        if (expected[i] == mem[i]) {
          result = false;
        }
      }

      return result;
    };

    REQUIRE(memory_check() == true);
  }
}
