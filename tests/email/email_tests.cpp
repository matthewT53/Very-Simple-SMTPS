#include <algorithm>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>

#include "doctest/doctest.h"

#include "email/email.hpp"
#include "mime/mime.hpp"

class MimeMock : public smtp::IMime {
public:
  MimeMock() {}

  void addAttachment(const std::string &p_file) override { m_attachments.push_back(p_file); }

  void removeAttachment(const std::string &p_file) override {
    auto it = std::find(m_attachments.begin(), m_attachments.end(), p_file);
    if (it != m_attachments.end()) {
      m_attachments.erase(it);
    }
  }

  void addMessage(const std::string &p_message) override { m_messages.push_back(p_message); }

  void removeMessage(const std::string &p_message) override {
    auto it = std::find(m_messages.begin(), m_messages.end(), p_message);
    if (it != m_messages.end()) {
      m_messages.erase(it);
    }
  }

  std::vector<std::string> build() const override {
    // mock().actualCall("build");

    std::vector<std::string> dummy;

    for (auto &message : m_messages) {
      dummy.push_back("\r\n");
      dummy.push_back(message);
      dummy.push_back("\r\n");
    }

    for (auto &file : m_attachments) {
      dummy.push_back("\r\n");
      dummy.push_back(file);
      dummy.push_back("\r\n");
    }

    dummy.push_back("\r\n.\r\n");

    return dummy;
  }

protected:
  std::ostream &output(std::ostream &p_out) const override {
    std::string result;
    for (const auto &line : build())
      result += line;
    return (p_out << m_dummy);
  }

private:
  std::string m_dummy;
  std::vector<std::string> m_attachments;
  std::vector<std::string> m_messages;
};

TEST_SUITE("Email tests") {
  TEST_CASE("Basic email test") {
    // mock().expectOneCall("build");
    smtp::Email email("user", "password", "hostname");

    std::unique_ptr<smtp::IMime> m = std::make_unique<MimeMock>();
    email.setMimeDocument(m);

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
    // mock().checkExpectations();
  }

  TEST_CASE("Add attachment test") {
    // mock().expectOneCall("build");
    smtp::Email email("user", "password", "hostname");

    std::unique_ptr<smtp::IMime> m = std::make_unique<MimeMock>();
    email.setMimeDocument(m);

    email.setTo("bigboss@gmail.com");
    email.setFrom("tully@gmail.com");
    email.setSubject("PWC pay rise");
    email.setCc("All the bosses at PWC");
    email.setBody("Hey mate, I have been working here for 5 years now, I think "
                  "its time for a pay rise.");
    email.addAttachment("MimeMockAttachment");

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

    REQUIRE(email_contents.size() > (expected_header.size() + expected_body.size()));
    REQUIRE(expected_header == email_contents.substr(0, expected_header.length()));
    REQUIRE(expected_body == email_contents.substr(
                                 email_contents.find("\r\nHey mate, I have been working here for 5 "
                                                     "years now, I think its time for a pay rise."),
                                 expected_body.length()));

    // mock().checkExpectations();
  }
}
