#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "doctest/doctest.h"

#include "date_time/date_time_now.hpp"
#include "email/email.hpp"
#include "mime/mime.hpp"

class DateTimeStatic : public smtp::DateTime {
public:
  std::string getTimestamp() const override { return "25/07/2023 07:21:05 +1100"; }
};

TEST_SUITE("Email tests") {
  TEST_CASE("Basic email test") {
    smtp::EmailParams params{"user", "password", "hostname"};
    smtp::Email email(params);

    email.setTo("bigboss@gmail.com");
    email.setFrom("tully@gmail.com");
    email.setSubject("PWC pay rise");
    email.setCc("All the bosses at PWC");
    email.setDate(DateTimeStatic());
    email.setBody("Hey mate, I have been working here for 5 years now, I think "
                  "its time for a pay rise.");

    const std::string &expected =
        "To: bigboss@gmail.com\r\n"
        "From: tully@gmail.com\r\n"
        "Cc: All the bosses at PWC\r\n"
        "Subject: PWC pay rise\r\n"
        "25/07/2023 07:21:05 +1100\r\n"
        "User-Agent: Very-Simple-SMTPS\r\n"
        "MIME-Version: 1.0\r\n"
        "Content-Type: multipart/mixed;\r\n"
        " boundary=\"----------030203080101020302070708\"\r\n"
        "\r\n"
        "This is a multi-part message in MIME format.\r\n"
        "------------030203080101020302070708\r\n"
        "Content-Type: text/plain; charset=utf-8; format=flowed\r\n"
        "Content-Transfer-Encoding: 7bit\r\n"
        "\r\n"
        "Hey mate, I have been working here for 5 years now, I think its time for a pay rise.\r\n"
        "------------030203080101020302070708\r\n"
        "------------030203080101020302070708--\r\n"
        ".\r\n";

    std::stringstream ss;
    ss << email;
    const std::string &actual = ss.str();
    REQUIRE(expected == actual);
  }

  TEST_CASE("Add attachment test") {
    smtp::EmailParams params{"user", "password", "hostname"};
    smtp::Email email(params);

    email.setTo("bigboss@gmail.com");
    email.setFrom("tully@gmail.com");
    email.setSubject("PWC pay rise");
    email.setDate(DateTimeStatic());
    email.setCc("All the bosses at PWC");
    email.setBody("Hey mate, I have been working here for 5 years now, I think "
                  "its time for a pay rise.");

    smtp::Attachment attachment;
    const std::string &contents = "MimeMockAttachment";
    attachment.setContents(std::vector<uint8_t>(contents.begin(), contents.end()));
    attachment.setFilePath("/path/test.txt");
    email.addAttachment(attachment);

    const std::string &expected =
        "To: bigboss@gmail.com\r\n"
        "From: tully@gmail.com\r\n"
        "Cc: All the bosses at PWC\r\n"
        "Subject: PWC pay rise\r\n"
        "25/07/2023 07:21:05 +1100\r\n"
        "User-Agent: Very-Simple-SMTPS\r\n"
        "MIME-Version: 1.0\r\n"
        "Content-Type: multipart/mixed;\r\n"
        " boundary=\"----------030203080101020302070708\"\r\n"
        "\r\n"
        "This is a multi-part message in MIME format.\r\n"
        "------------030203080101020302070708\r\n"
        "Content-Type: text/plain; charset=utf-8; format=flowed\r\n"
        "Content-Transfer-Encoding: 7bit\r\n"
        "\r\n"
        "Hey mate, I have been working here for 5 years now, I think its time for a pay rise.\r\n"
        "------------030203080101020302070708\r\n"
        "Content-Type: application/octet-stream\r\n"
        "Content-Transfer-Encoding: base64\r\n"
        "Content-Disposition: attachment;\r\n"
        " filename=test.txt\r\n"
        "\r\n"
        "TWltZU1vY2tBdHRhY2htZW50\r\n"
        "\r\n"
        "------------030203080101020302070708\r\n"
        "------------030203080101020302070708--\r\n"
        ".\r\n";

    std::stringstream ss;
    ss << email;
    const std::string &actual = ss.str();

    REQUIRE(expected == actual);
  }

  TEST_CASE("Remove attachment test") {
    smtp::EmailParams params{"user", "password", "hostname"};
    smtp::Email email(params);

    email.setTo("bigboss@gmail.com");
    email.setFrom("tully@gmail.com");
    email.setSubject("PWC pay rise");
    email.setDate(DateTimeStatic());
    email.setCc("All the bosses at PWC");
    email.setBody("Hey mate, I have been working here for 5 years now, I think "
                  "its time for a pay rise.");

    smtp::Attachment attachment_one;
    const std::string &contents_one = "MimeMockAttachment";
    attachment_one.setContents(std::vector<uint8_t>(contents_one.begin(), contents_one.end()));
    attachment_one.setFilePath("/path/test1.txt");
    email.addAttachment(attachment_one);

    smtp::Attachment attachment_two;
    const std::string &contents_two = "MimeMockAttachment2";
    attachment_two.setContents(std::vector<uint8_t>(contents_two.begin(), contents_two.end()));
    attachment_two.setFilePath("/path/test2.txt");
    email.addAttachment(attachment_two);

    email.removeAttachment("/path/test1.txt");

    const std::string &expected =
        "To: bigboss@gmail.com\r\n"
        "From: tully@gmail.com\r\n"
        "Cc: All the bosses at PWC\r\n"
        "Subject: PWC pay rise\r\n"
        "25/07/2023 07:21:05 +1100\r\n"
        "User-Agent: Very-Simple-SMTPS\r\n"
        "MIME-Version: 1.0\r\n"
        "Content-Type: multipart/mixed;\r\n"
        " boundary=\"----------030203080101020302070708\"\r\n"
        "\r\n"
        "This is a multi-part message in MIME format.\r\n"
        "------------030203080101020302070708\r\n"
        "Content-Type: text/plain; charset=utf-8; format=flowed\r\n"
        "Content-Transfer-Encoding: 7bit\r\n"
        "\r\n"
        "Hey mate, I have been working here for 5 years now, I think its time for a pay rise.\r\n"
        "------------030203080101020302070708\r\n"
        "Content-Type: application/octet-stream\r\n"
        "Content-Transfer-Encoding: base64\r\n"
        "Content-Disposition: attachment;\r\n"
        " filename=test2.txt\r\n"
        "\r\n"
        "TWltZU1vY2tBdHRhY2htZW50Mg==\r\n"
        "\r\n"
        "------------030203080101020302070708\r\n"
        "------------030203080101020302070708--\r\n"
        ".\r\n";

    std::stringstream ss;
    ss << email;
    const std::string &actual = ss.str();

    REQUIRE(expected == actual);
  }

  TEST_CASE("Clear test") {
    smtp::EmailParams params{"user", "password", "hostname"};
    smtp::Email email(params);

    email.setTo("bigboss@gmail.com");
    email.setFrom("tully@gmail.com");
    email.setSubject("PWC pay rise");
    email.setCc("All the bosses at PWC");
    email.setDate(DateTimeStatic());
    email.setBody("Hey mate, I have been working here for 5 years now, I think "
                  "its time for a pay rise.");

    smtp::Attachment attachment_one;
    const std::string &contents_one = "MimeMockAttachment";
    attachment_one.setContents(std::vector<uint8_t>(contents_one.begin(), contents_one.end()));
    email.addAttachment(attachment_one);

    email.clear();
    email.setDate(DateTimeStatic());

    const std::string &expected = "To: \r\n"
                                  "From: \r\n"
                                  "Cc: \r\n"
                                  "Subject: \r\n"
                                  "25/07/2023 07:21:05 +1100\r\n"
                                  "User-Agent: Very-Simple-SMTPS\r\n"
                                  "MIME-Version: 1.0\r\n"
                                  "Content-Type: multipart/mixed;\r\n"
                                  " boundary=\"----------030203080101020302070708\"\r\n"
                                  "\r\n"
                                  "This is a multi-part message in MIME format.\r\n"
                                  "------------030203080101020302070708\r\n"
                                  "Content-Type: text/plain; charset=utf-8; format=flowed\r\n"
                                  "Content-Transfer-Encoding: 7bit\r\n"
                                  "\r\n"
                                  "\r\n"
                                  "------------030203080101020302070708\r\n"
                                  "------------030203080101020302070708--\r\n"
                                  ".\r\n";

    std::stringstream ss;
    ss << email;
    const std::string &actual = ss.str();

    REQUIRE(expected == actual);
  }
}
