#include "CppUTest/TestHarness.h"

#include "base64.h"

TEST_GROUP( Base64EncodingTests )
{
};

TEST( Base64EncodingTests, BasicEncodingTest )
{
    const std::string text = "aaa";
    const std::string encoded = SMTP::Base64::Base64_Encode(text);
    CHECK_EQUAL("YWFh", encoded);
}

TEST( Base64EncodingTests, PaddingTest ) 
{
    const std::string text = "aaaa";
    const std::string encoded = SMTP::Base64::Base64_Encode(text);
    CHECK_EQUAL("YWFhYQo=", encoded); 
}

TEST( Base64EncodingTests, EmptyStringTest )
{
    const std::string encoded = SMTP::Base64::Base64_Encode("");
    CHECK_EQUAL("", encoded);
}

TEST_GROUP( Base64UrlEncodingTests )
{
};

TEST_GROUP( Base64DecodingTests )
{
};

TEST_GROUP( Base64UrlDecodingTests )
{
};

