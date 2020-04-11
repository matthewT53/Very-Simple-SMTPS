#include "CppUTest/TestHarness.h"

#include <vector>
#include <cstdint>

#include "base64.h"

using byte = uint8_t;

TEST_GROUP( Base64EncodingTests )
{
};

TEST( Base64EncodingTests, BasicEncodingTest )
{
    const std::vector<byte> data = { 'a', 'a', 'a' };
    const std::string encoded = smtp::Base64::Base64_Encode( data );
    CHECK_EQUAL( "YWFh", encoded );
}

TEST( Base64EncodingTests, OnePaddingCharacterTest ) 
{
    const std::vector<byte> data = { 'a', 'a', 'a', 'a', 'a' };
    const std::string encoded = smtp::Base64::Base64_Encode( data );
    CHECK_EQUAL( "YWFhYWE=", encoded );
}

TEST( Base64EncodingTests, TwoPaddingCharacterTest )
{
    const std::vector<byte> data = { 'a', 'a', 'a', 'a' };
    const std::string encoded = smtp::Base64::Base64_Encode( data );
    CHECK_EQUAL( "YWFhYQ==", encoded ); 
}

TEST( Base64EncodingTests, EmptyStringTest )
{
    const std::string encoded = smtp::Base64::Base64_Encode( {} );
    CHECK_EQUAL( "", encoded );
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

