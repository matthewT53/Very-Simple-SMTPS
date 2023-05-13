#include "CppUTest/TestHarness.h"

#include <cstdint>
#include <iostream>
#include <vector>

#include "base64.hpp"

using byte = uint8_t;

TEST_GROUP(Base64EncodingTests){};

TEST(Base64EncodingTests, BasicEncodingTest) {
  const std::vector<byte> data = {'a', 'a', 'a'};
  const std::string encoded = smtp::Base64::Base64Encode(data);

  CHECK_EQUAL(std::string("YWFh"), encoded);
}

TEST(Base64EncodingTests, OnePaddingCharacterTest) {
  const std::vector<byte> data = {'a', 'a', 'a', 'a', 'a'};
  const std::string encoded = smtp::Base64::Base64Encode(data);

  CHECK_EQUAL(std::string("YWFhYWE="), encoded);
}

TEST(Base64EncodingTests, TwoPaddingCharacterTest) {
  const std::vector<byte> data = {'a', 'a', 'a', 'a'};
  const std::string encoded = smtp::Base64::Base64Encode(data);

  CHECK_EQUAL(std::string("YWFhYQ=="), encoded);
}

TEST(Base64EncodingTests, TypicalExampleTest) {
  const std::string text = "what does it take for a boar to soar?";
  const std::vector<byte> data(text.begin(), text.end());
  const std::string encoded = smtp::Base64::Base64Encode(data);

  CHECK_EQUAL(std::string("d2hhdCBkb2VzIGl0IHRha2UgZm9yIGEgYm9hciB0byBzb2FyPw=="), encoded);
}

TEST(Base64EncodingTests, BinaryDataTest) {
  const std::vector<byte> data = {0x90, 0x90, 0x90, 0x80, 0x75, 0x12, 0xa};
  const std::string encoded = smtp::Base64::Base64Encode(data);

  CHECK_EQUAL(std::string("kJCQgHUSCg=="), encoded);
}

TEST(Base64EncodingTests, EmptyStringTest) {
  const std::string encoded = smtp::Base64::Base64Encode({});
  CHECK(encoded.empty());
}

TEST_GROUP(Base64UrlEncodingTests){};

TEST(Base64UrlEncodingTests, BasicEncodingTest) {
  const std::string text = "hey mate how are you doing? good day today?";
  const std::vector<byte> data(text.begin(), text.end());
  const std::string encoded = smtp::Base64::Base64UrlEncode(data);

  CHECK_EQUAL(std::string("aGV5IG1hdGUgaG93IGFyZSB5b3UgZG9pbmc_IGdvb2QgZGF5IHRvZGF5Pw"), encoded);
}

TEST(Base64UrlEncodingTests, PaddingRemovalTest) {
  const std::string text = "what is the time?";
  const std::vector<byte> data(text.begin(), text.end());
  const std::string encoded = smtp::Base64::Base64UrlEncode(data);

  CHECK_EQUAL(std::string("d2hhdCBpcyB0aGUgdGltZT8"), encoded);
}

TEST_GROUP(Base64DecodingTests){};

TEST(Base64DecodingTests, BasicDecodingTest) {
  const std::string data = "YWFh";
  const std::vector<byte> data_bytes = smtp::Base64::Base64Decode(data);
  const std::string result(data_bytes.begin(), data_bytes.end());

  CHECK_EQUAL(std::string("aaa"), result);
}

TEST(Base64DecodingTests, DecodePaddingTest) {
  const std::string data = "QXJlIHdlIHJlYWxseSBmcmVlPw==";
  const std::vector<byte> data_bytes = smtp::Base64::Base64Decode(data);
  const std::string result(data_bytes.begin(), data_bytes.end());

  CHECK_EQUAL(std::string("Are we really free?"), result);
}

TEST(Base64DecodingTests, BinaryDataDecodeTest) {
  const std::string data = "hnOQkJCAdRIICg==";
  const std::vector<byte> result = smtp::Base64::Base64Decode(data);
  const std::vector<byte> expected = {0x86, 0x73, 0x90, 0x90, 0x90, 0x80, 0x75, 0x12, 0x08, 0xa};

  CHECK(expected == result);
}

TEST(Base64DecodingTests, DecodeOnePaddingTest) {
  const std::string data = "V2hhdCBhcmUgeW91IGRvaW5nIGZyaWVuZD8=";
  const std::vector<byte> data_bytes = smtp::Base64::Base64Decode(data);
  const std::string result(data_bytes.begin(), data_bytes.end());

  CHECK_EQUAL(std::string("What are you doing friend?"), result);
}

TEST(Base64DecodingTests, EmptyStringTest) {
  const std::vector<byte> result = smtp::Base64::Base64Decode("");
  CHECK(result.empty());
}

TEST_GROUP(Base64UrlDecodingTests){};

TEST(Base64UrlDecodingTests, BasicDecodingTest) {
  const std::string data = "aGV5IG1hdGUgaG93IGFyZSB5b3UgZG9pbmc_IGdvb2QgZGF5IHRvZGF5Pw";
  const std::vector<byte> result = smtp::Base64::Base64UrlDecode(data);
  const std::string actual(result.begin(), result.end());

  CHECK_EQUAL(std::string("hey mate how are you doing? good day today?"), actual);
}

TEST(Base64UrlDecodingTests, DecodePaddingTest) {
  const std::string data = "V2hhdCBkb2VzIGl0IHRha2UgZm9yIGEgYm9hciB0byBzb2FyIGl"
                           "uIE5ldyBZb3JrLiBBcmUgeW91IHdlbGwgdG9kYXkgc"
                           "2lyPz8_"
                           "IFdlbGwgaWYgeW91IGFyZSB0aGVuIHlvdSBwbGVhc2UgdmlzaXQ"
                           "gbWUgaW4gbmV3IHlvcms_IFNlZSB5b3Ugc29vbg";
  const std::vector<byte> result = smtp::Base64::Base64UrlDecode(data);
  const std::string actual(result.begin(), result.end());

  CHECK_EQUAL(std::string("What does it take for a boar to soar in New York. "
                          "Are you well today sir??? Well if you are then you "
                          "please visit me in new york? See you soon"),
              actual);
}

TEST(Base64UrlDecodingTests, DecodeWithPaddingTest) {
  const std::string data = "V2hhdCBkb2VzIGl0IHRha2UgZm9yIGEgYm9hciB0byBzb2FyIGl"
                           "uIE5ldyBZb3JrLiBBcmUgeW91IHdlbGwgdG9kYXkgc"
                           "2lyPz8_"
                           "IFdlbGwgaWYgeW91IGFyZSB0aGVuIHlvdSBwbGVhc2UgdmlzaXQ"
                           "gbWUgaW4gbmV3IHlvcms_IFNlZSB5b3Ugc29vbg==";
  const std::vector<byte> result = smtp::Base64::Base64UrlDecode(data);
  const std::string actual(result.begin(), result.end());

  CHECK_EQUAL(std::string("What does it take for a boar to soar in New York. "
                          "Are you well today sir??? Well if you are then you "
                          "please visit me in new york? See you soon"),
              actual);
}

TEST(Base64UrlDecodingTests, EmptyStringTest) {
  const std::vector<byte> result = smtp::Base64::Base64UrlDecode("");
  CHECK(result.empty());
}
