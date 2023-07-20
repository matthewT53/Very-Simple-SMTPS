#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "doctest/doctest.h"

#include "email/email.hpp"
#include "mime/mime.hpp"

TEST_SUITE("Email tests") {
  TEST_CASE("Basic email test") {
    smtp::EmailParams params{"user", "password", "hostname"};
    smtp::Email email(params);

    email.setTo("bigboss@gmail.com");
    email.setFrom("tully@gmail.com");
    email.setSubject("PWC pay rise");
    email.setCc("All the bosses at PWC");
    email.setBody("Hey mate, I have been working here for 5 years now, I think "
                  "its time for a pay rise.");

    std::stringstream ss;
    ss << email;

    const std::string &expected_header = "To: bigboss@gmail.com\r\n"
                                         "From: tully@gmail.com\r\n"
                                         "Cc: All the bosses at PWC\r\n"
                                         "Subject: PWC pay rise\r\n";

    const std::string &expected_body =
        "\r\n"
        "Hey mate, I have been working here for 5 years now, I think its time "
        "for a pay rise."
        "\r\n"
        // This is where the attachments will go if there are any.
        "\r\n.\r\n";

    const std::string &email_contents = ss.str();
    REQUIRE(expected_header == email_contents.substr(0, expected_header.length()));
    REQUIRE(expected_body == email_contents.substr(
                                 email_contents.find("\r\nHey mate, I have been working here for 5 "
                                                     "years now, I think its time for a pay rise."),
                                 expected_body.length()));
  }

  TEST_CASE("Add attachment test") {
    smtp::EmailParams params{"user", "password", "hostname"};
    smtp::Email email(params);

    email.setTo("bigboss@gmail.com");
    email.setFrom("tully@gmail.com");
    email.setSubject("PWC pay rise");
    email.setCc("All the bosses at PWC");
    email.setBody("Hey mate, I have been working here for 5 years now, I think "
                  "its time for a pay rise.");

    smtp::Attachment attachment;
    const std::string &contents = "MimeMockAttachment";
    attachment.setContents(std::vector<uint8_t>(contents.begin(), contents.end()));
    email.addAttachment(attachment);

    std::stringstream ss;
    ss << email;

    const std::string &expected_header = "To: bigboss@gmail.com\r\n"
                                         "From: tully@gmail.com\r\n"
                                         "Cc: All the bosses at PWC\r\n"
                                         "Subject: PWC pay rise\r\n";

    const std::string &expected_body =
        "\r\n"
        "Hey mate, I have been working here for 5 years now, I think its time "
        "for a pay rise."
        "\r\n"
        "\r\n"
        "MimeMockAttachment"
        "\r\n"
        "\r\n.\r\n";

    const std::string &email_contents = ss.str();

    REQUIRE(expected_header == email_contents.substr(0, expected_header.length()));
    REQUIRE(expected_body == email_contents.substr(
                                 email_contents.find("\r\nHey mate, I have been working here for 5 "
                                                     "years now, I think its time for a pay rise."),
                                 expected_body.length()));
  }

  TEST_CASE("Remove attachment test") {
    smtp::EmailParams params{"user", "password", "hostname"};
    smtp::Email email(params);

    email.setTo("bigboss@gmail.com");
    email.setFrom("tully@gmail.com");
    email.setSubject("PWC pay rise");
    email.setCc("All the bosses at PWC");
    email.setBody("Hey mate, I have been working here for 5 years now, I think "
                  "its time for a pay rise.");

    smtp::Attachment attachment_one;
    const std::string &contents_one = "MimeMockAttachment";
    attachment_one.setContents(std::vector<uint8_t>(contents_one.begin(), contents_one.end()));
    email.addAttachment(attachment_one);

    smtp::Attachment attachment_two;
    const std::string &contents_two = "MimeMockAttachment2";
    attachment_two.setContents(std::vector<uint8_t>(contents_two.begin(), contents_two.end()));
    email.addAttachment(attachment_two);

    email.removeAttachment(attachment_one);

    std::stringstream ss;
    ss << email;

    const std::string &expected_header = "To: bigboss@gmail.com\r\n"
                                         "From: tully@gmail.com\r\n"
                                         "Cc: All the bosses at PWC\r\n"
                                         "Subject: PWC pay rise\r\n";

    const std::string &expected_body =
        "\r\n"
        "Hey mate, I have been working here for 5 years now, I think its time "
        "for a pay rise."
        "\r\n"
        "\r\n"
        "MimeMockAttachment2"
        "\r\n"
        "\r\n.\r\n";

    const std::string &email_contents = ss.str();

    REQUIRE(expected_header == email_contents.substr(0, expected_header.length()));
    REQUIRE(expected_body == email_contents.substr(
                                 email_contents.find("\r\nHey mate, I have been working here for 5 "
                                                     "years now, I think its time for a pay rise."),
                                 expected_body.length()));
  }

  TEST_CASE("Clear test") {
    smtp::EmailParams params{"user", "password", "hostname"};
    smtp::Email email(params);

    email.setTo("bigboss@gmail.com");
    email.setFrom("tully@gmail.com");
    email.setSubject("PWC pay rise");
    email.setCc("All the bosses at PWC");
    email.setBody("Hey mate, I have been working here for 5 years now, I think "
                  "its time for a pay rise.");

    smtp::Attachment attachment_one;
    const std::string &contents_one = "MimeMockAttachment";
    attachment_one.setContents(std::vector<uint8_t>(contents_one.begin(), contents_one.end()));
    email.addAttachment(attachment_one);

    email.clear();

    std::stringstream ss;
    ss << email;

    const std::string &expected_email = "To: \r\n"
                                        "From: \r\n"
                                        "Cc: \r\n"
                                        "Subject: \r\n";

    const std::string &email_contents = ss.str();

    REQUIRE(expected_email == email_contents);
  }
}
