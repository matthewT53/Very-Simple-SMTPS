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

TEST( Base64EncodingTests, TypicalExampleTest )
{
    const std::string text = "what does it take for a boar to soar?";
    const std::vector<byte> data( text.begin(), text.end() );
    const std::string encoded = smtp::Base64::Base64_Encode( data );

    CHECK_EQUAL( "d2hhdCBkb2VzIGl0IHRha2UgZm9yIGEgYm9hciB0byBzb2FyPw==", encoded );
}

TEST( Base64EncodingTests, EmptyStringTest )
{
    const std::string encoded = smtp::Base64::Base64_Encode( {} );
    CHECK_EQUAL( "", encoded );
}

TEST_GROUP( Base64UrlEncodingTests )
{
};

TEST( Base64UrlEncodingTests, BasicEncodingTest )
{
    const std::string text = "hey mate how are you doing? good day today?";
    const std::vector<byte> data( text.begin(), text.end() );
    const std::string encoded = smtp::Base64::Base64Url_Encode( data );

    CHECK_EQUAL( "aGV5IG1hdGUgaG93IGFyZSB5b3UgZG9pbmc_IGdvb2QgZGF5IHRvZGF5Pw", encoded );
}

TEST( Base64UrlEncodingTests, PaddingRemovalTest )
{
    const std::string text = "what is the time?";
    const std::vector<byte> data( text.begin(), text.end() );
    const std::string encoded = smtp::Base64::Base64Url_Encode( data );

    CHECK_EQUAL( "d2hhdCBpcyB0aGUgdGltZT8", encoded );
}

TEST_GROUP( Base64DecodingTests )
{
};

TEST_GROUP( Base64UrlDecodingTests )
{
};

