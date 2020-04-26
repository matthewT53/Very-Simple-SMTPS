#include "CppUTest/TestHarness.h"

#include <iostream>
#include <string>
#include <sstream>

#include "mime.h"

TEST_GROUP( MimeTests )
{
};

TEST( MimeTests, BasicUseTest )
{
    smtp::Mime m;
    std::stringstream s;

    s << m;

}

TEST( MimeTests, TextFileAttachmentTest )
{

}

TEST( MimeTests, BinaryAttachmentTest )
{

}

TEST( MimeTests, MultipleAttachmentTest )
{
    
}