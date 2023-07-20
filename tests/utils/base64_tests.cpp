#include "doctest/doctest.h"

#include <cstdint>
#include <iostream>
#include <vector>

#include "utils/base64/base64.hpp"

using byte = uint8_t;

using namespace smtp;

TEST_SUITE("Basic b64 encoding") {
  TEST_CASE("Basic encoding test") {
    const std::vector<byte> data = {'a', 'a', 'a'};
    const std::string encoded = Base64::Base64Encode(data);

    REQUIRE(encoded == "YWFh");
  }

  TEST_CASE("One padding character test") {
    const std::vector<byte> data = {'a', 'a', 'a', 'a', 'a'};
    const std::string encoded = Base64::Base64Encode(data);

    REQUIRE(encoded == "YWFhYWE=");
  }

  TEST_CASE("Two padding character test") {
    const std::vector<byte> data = {'a', 'a', 'a', 'a'};
    const std::string encoded = Base64::Base64Encode(data);

    REQUIRE(encoded == "YWFhYQ==");
  }

  TEST_CASE("Basic example") {
    const std::string text = "what does it take for a boar to soar?";
    const std::vector<byte> data(text.begin(), text.end());
    const std::string encoded = Base64::Base64Encode(data);

    REQUIRE(encoded == "d2hhdCBkb2VzIGl0IHRha2UgZm9yIGEgYm9hciB0byBzb2FyPw==");
  }

  TEST_CASE("Encoding binary data test") {
    const std::vector<byte> data = {0x90, 0x90, 0x90, 0x80, 0x75, 0x12, 0xa};
    const std::string encoded = Base64::Base64Encode(data);

    REQUIRE(encoded == "kJCQgHUSCg==");
  }

  TEST_CASE("Encoding an empty string") {
    const std::string encoded = Base64::Base64Encode("");
    CHECK(encoded.empty());
  }

  TEST_CASE("Basic encoding with string argument") {
    const std::string &s = "encode me please";
    const std::string &encoded = Base64::Base64Encode(s);
    REQUIRE(encoded == "ZW5jb2RlIG1lIHBsZWFzZQo=");
  }
}

TEST_SUITE("Base64 url encoding") {
  TEST_CASE("Basic url encoding") {
    const std::string text = "hey mate how are you doing? good day today?";
    const std::vector<byte> data(text.begin(), text.end());
    const std::string encoded = Base64::Base64UrlEncode(data);

    REQUIRE(encoded == "aGV5IG1hdGUgaG93IGFyZSB5b3UgZG9pbmc_IGdvb2QgZGF5IHRvZGF5Pw");
  }

  TEST_CASE("Padding is removed") {
    const std::string text = "what is the time?";
    const std::vector<byte> data(text.begin(), text.end());
    const std::string encoded = Base64::Base64UrlEncode(data);

    REQUIRE(encoded == "d2hhdCBpcyB0aGUgdGltZT8");
  }
}

TEST_SUITE("Base64 decoding") {
  TEST_CASE("Basic decoding test") {
    const std::string data = "YWFh";
    const std::vector<byte> data_bytes = Base64::Base64Decode(data);
    const std::string result(data_bytes.begin(), data_bytes.end());

    REQUIRE(result == "aaa");
  }

  TEST_CASE("Decode string with padding characters") {
    const std::string data = "QXJlIHdlIHJlYWxseSBmcmVlPw==";
    const std::vector<byte> data_bytes = Base64::Base64Decode(data);
    const std::string result(data_bytes.begin(), data_bytes.end());

    REQUIRE(result == "Are we really free?");
  }

  TEST_CASE("Binary data decode test") {
    const std::string data = "hnOQkJCAdRIICg==";
    const std::vector<byte> result = Base64::Base64Decode(data);
    const std::vector<byte> expected = {0x86, 0x73, 0x90, 0x90, 0x90, 0x80, 0x75, 0x12, 0x08, 0xa};

    REQUIRE(result == expected);
  }

  TEST_CASE("Decode string with one padding character") {
    const std::string data = "V2hhdCBhcmUgeW91IGRvaW5nIGZyaWVuZD8=";
    const std::vector<byte> data_bytes = Base64::Base64Decode(data);
    const std::string result(data_bytes.begin(), data_bytes.end());

    REQUIRE(result == "What are you doing friend?");
  }

  TEST_CASE("Empty string test") {
    const std::vector<byte> result = Base64::Base64Decode("");
    REQUIRE(result.empty());
  }
}

TEST_SUITE("Base64 url decoding tests") {
  TEST_CASE("Basic base64 url decoding test") {
    const std::string data = "aGV5IG1hdGUgaG93IGFyZSB5b3UgZG9pbmc_IGdvb2QgZGF5IHRvZGF5Pw";
    const std::vector<byte> result = Base64::Base64UrlDecode(data);
    const std::string actual(result.begin(), result.end());

    REQUIRE(actual == "hey mate how are you doing? good day today?");
  }

  TEST_CASE("Decode ba64 url encoded string that has no padding") {
    const std::string data =
        "V2hhdCBkb2VzIGl0IHRha2UgZm9yIGEgYm9hciB0byBzb2FyIGluIE5ldyBZb3JrLiBBcmUgeW91IHdlbGwgdG9kYX"
        "kgc"
        "2lyPz8_IFdlbGwgaWYgeW91IGFyZSB0aGVuIHlvdSBwbGVhc2UgdmlzaXQgbWUgaW4gbmV3IHlvcms_"
        "IFNlZSB5b3Ugc29vbg";
    const std::vector<byte> result = Base64::Base64UrlDecode(data);
    const std::string actual(result.begin(), result.end());

    REQUIRE(actual == "What does it take for a boar to soar in New York. Are you well today sir??? "
                      "Well if you are then you please visit me in new york? See you soon");
  }

  TEST_CASE("Decode b64 url encoded string that has padding") {
    const std::string data =
        "V2hhdCBkb2VzIGl0IHRha2UgZm9yIGEgYm9hciB0byBzb2FyIGluIE5ldyBZb3JrLiBBcmUgeW91IHdlbGwgdG9kYX"
        "kgc"
        "2lyPz8_IFdlbGwgaWYgeW91IGFyZSB0aGVuIHlvdSBwbGVhc2UgdmlzaXQgbWUgaW4gbmV3IHlvcms_"
        "IFNlZSB5b3Ugc29vbg==";
    const std::vector<byte> result = Base64::Base64UrlDecode(data);
    const std::string actual(result.begin(), result.end());

    REQUIRE(actual == "What does it take for a boar to soar in New York. Are you well today sir??? "
                      "Well if you are then you please visit me in new york? See you soon");
  }

  TEST_CASE("Empty string test") {
    const std::vector<byte> result = Base64::Base64UrlDecode("");
    REQUIRE(result.empty());
  }
}
