#include "doctest/doctest.h"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "mime/mime.hpp"

namespace fs = std::filesystem;

static const std::string &kTextPath = "test_data/test.txt";
static const std::string &kBinaryPath = "test_data/test.bin";
static const std::string &kLargeBinaryPath = "test_data/large.bin";

class MimeTestsFixture {
public:
  MimeTestsFixture() {
    std::ofstream text_file(kTextPath);
    if (!text_file.is_open()) {
      throw std::runtime_error("Unable to open: " + kTextPath);
    }
    text_file << "This is some test data for the file.";
    text_file.close();

    std::ofstream binary_file(kBinaryPath);
    if (!binary_file) {
      throw std::runtime_error("Unable to open: " + kBinaryPath);
    }

    binary_file << "\x90\x12\x87\x85\x43\x65\x10\x12\x65\x90\x34\x23\x25\x65\x41"
                   "\x42\x43\xf9";
    binary_file.close();

    std::ofstream large_file(kLargeBinaryPath);
    if (!large_file) {
      throw std::runtime_error("Unable to open: " + kLargeBinaryPath);
    }

    std::string s;
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 256; j++) {
        s.push_back(static_cast<char>(j));
      }
    }

    large_file << s;
    large_file.close();
  }
};

TEST_CASE_FIXTURE(MimeTestsFixture, "Basic use test") {
  smtp::Mime m("test_user_agent");
  std::stringstream ss;
  ss << m;

  const std::string &actual = ss.str();
  const std::string &expected = "";

  REQUIRE(expected == actual);
}

TEST_CASE_FIXTURE(MimeTestsFixture, "No attachments test") {
  const std::string &message = "This is a test message placed inside the body.";
  smtp::Mime m("test_user_agent");
  m.addMessage(message);
  m.build();

  std::stringstream ss;
  ss << m;

  const std::string &actual = ss.str();
  const std::string &expected = "User-Agent: test_user_agent\r\n"
                                "MIME-Version: 1.0\r\n"
                                "Content-Type: multipart/mixed;\r\n"
                                " boundary=\"" +
                                smtp::Mime::kBoundaryDeclare + "\"" + "\r\n" +
                                "\r\nThis is a multi-part message in MIME format.\r\n" +
                                smtp::Mime::kBoundary +
                                "\r\n"
                                "Content-Type: text/plain; charset=utf-8; format=flowed\r\n"
                                "Content-Transfer-Encoding: 7bit\r\n"
                                "\r\n" +
                                message + "\r\n" + smtp::Mime::kBoundary + "\r\n";

  REQUIRE(expected == actual);
}

TEST_CASE_FIXTURE(MimeTestsFixture, "Text file attachment test") {
  smtp::Mime m("test_user_agent");
  m.addAttachment(kTextPath);
  m.build();

  std::stringstream ss;
  ss << m;

  const std::string &text_filename = fs::path(kTextPath).filename().string();

  const std::string &actual = ss.str();
  const std::string &expected = "User-Agent: test_user_agent\r\n"
                                "MIME-Version: 1.0\r\n"
                                "Content-Type: multipart/mixed;\r\n"
                                " boundary=\"" +
                                smtp::Mime::kBoundaryDeclare + "\"" + "\r\n" +
                                "\r\nThis is a multi-part message in MIME format.\r\n" +
                                smtp::Mime::kBoundary +
                                "\r\n"
                                "Content-Type: application/octet-stream\r\n"
                                "Content-Transfer-Encoding: base64\r\n"
                                "Content-Disposition: attachment;\r\n"
                                " filename=" +
                                text_filename +
                                "\r\n"
                                "\r\n"
                                "VGhpcyBpcyBzb21lIHRlc3QgZGF0YSBmb3IgdGhlIGZpbGUu"
                                "\r\n\r\n" +
                                smtp::Mime::kBoundary + "\r\n";

  REQUIRE(expected == actual);
}

TEST_CASE_FIXTURE(MimeTestsFixture, "Binary attachment test") {
  smtp::Mime m("test_user_agent");
  m.addAttachment(kBinaryPath);
  m.build();

  std::stringstream ss;
  ss << m;

  const std::string &bin_filename = fs::path(kBinaryPath).filename().string();
  const std::string &actual = ss.str();
  const std::string &expected = "User-Agent: test_user_agent\r\n"
                                "MIME-Version: 1.0\r\n"
                                "Content-Type: multipart/mixed;\r\n"
                                " boundary=\"" +
                                smtp::Mime::kBoundaryDeclare + "\"" + "\r\n" +
                                "\r\nThis is a multi-part message in MIME format.\r\n" +
                                smtp::Mime::kBoundary +
                                "\r\n"
                                "Content-Type: application/octet-stream\r\n"
                                "Content-Transfer-Encoding: base64\r\n"
                                "Content-Disposition: attachment;\r\n"
                                " filename=" +
                                bin_filename +
                                "\r\n"
                                "\r\n"
                                "kBKHhUNlEBJlkDQjJWVBQkP5\r\n"
                                "\r\n" +
                                smtp::Mime::kBoundary + "\r\n";

  REQUIRE(expected == actual);
}

TEST_CASE_FIXTURE(MimeTestsFixture, "Multiple attachment test") {
  smtp::Mime m("test_user_agent");
  m.addAttachment(kTextPath);
  m.addAttachment(kBinaryPath);
  m.build();

  std::stringstream ss;
  ss << m;

  const std::string &text_filename = fs::path(kTextPath).filename().string();
  const std::string &bin_filename = fs::path(kBinaryPath).filename().string();

  const std::string &actual = ss.str();
  const std::string &expected = "User-Agent: test_user_agent\r\n"
                                "MIME-Version: 1.0\r\n"
                                "Content-Type: multipart/mixed;\r\n"
                                " boundary=\"" +
                                smtp::Mime::kBoundaryDeclare + "\"" + "\r\n" +
                                "\r\nThis is a multi-part message in MIME format.\r\n" +
                                smtp::Mime::kBoundary +
                                "\r\n"
                                "Content-Type: application/octet-stream\r\n"
                                "Content-Transfer-Encoding: base64\r\n"
                                "Content-Disposition: attachment;\r\n"
                                " filename=" +
                                text_filename +
                                "\r\n"
                                "\r\n"
                                "VGhpcyBpcyBzb21lIHRlc3QgZGF0YSBmb3IgdGhlIGZpbGUu\r\n"
                                "\r\n" +
                                smtp::Mime::kBoundary +
                                "\r\n"
                                "Content-Type: application/octet-stream\r\n"
                                "Content-Transfer-Encoding: base64\r\n"
                                "Content-Disposition: attachment;\r\n"
                                " filename=" +
                                bin_filename +
                                "\r\n"
                                "\r\n"
                                "kBKHhUNlEBJlkDQjJWVBQkP5\r\n"
                                "\r\n" +
                                smtp::Mime::kBoundary + "\r\n";

  REQUIRE(expected == actual);
}

TEST_CASE_FIXTURE(MimeTestsFixture, "Remove attachment test") {
  smtp::Mime m("test_user_agent");
  m.addAttachment(kTextPath);
  m.addAttachment(kBinaryPath);
  m.removeAttachment(kTextPath);
  m.build();

  std::stringstream ss;
  ss << m;

  const std::string &bin_filename = fs::path(kBinaryPath).filename().string();

  const std::string &actual = ss.str();
  const std::string &expected = "User-Agent: test_user_agent\r\n"
                                "MIME-Version: 1.0\r\n"
                                "Content-Type: multipart/mixed;\r\n"
                                " boundary=\"" +
                                smtp::Mime::kBoundaryDeclare + "\"" + "\r\n" +
                                "\r\nThis is a multi-part message in MIME format.\r\n" +
                                smtp::Mime::kBoundary +
                                "\r\n"
                                "Content-Type: application/octet-stream\r\n"
                                "Content-Transfer-Encoding: base64\r\n"
                                "Content-Disposition: attachment;\r\n"
                                " filename=" +
                                bin_filename +
                                "\r\n"
                                "\r\n"
                                "kBKHhUNlEBJlkDQjJWVBQkP5\r\n"
                                "\r\n" +
                                smtp::Mime::kBoundary + "\r\n";

  REQUIRE(expected == actual);
}

TEST_CASE_FIXTURE(MimeTestsFixture, "Very large attachment test") {
  smtp::Mime m("test_user_agent");
  m.addAttachment(kLargeBinaryPath);
  m.build();

  std::stringstream ss;
  ss << m;

  const std::string &bin_filename = fs::path(kLargeBinaryPath).filename().string();

  const std::string &actual = ss.str();
  const std::string &expected =
      "User-Agent: test_user_agent\r\n"
      "MIME-Version: 1.0\r\n"
      "Content-Type: multipart/mixed;\r\n"
      " boundary=\"" +
      smtp::Mime::kBoundaryDeclare + "\"" + "\r\n" +
      "\r\nThis is a multi-part message in MIME format.\r\n" + smtp::Mime::kBoundary +
      "\r\n"
      "Content-Type: application/octet-stream\r\n"
      "Content-Transfer-Encoding: base64\r\n"
      "Content-Disposition: attachment;\r\n"
      " filename=" +
      bin_filename +
      "\r\n"
      "\r\n"
      "AAECAwQFBgcICQoLDA0ODxAREhMUFRYXGBkaGxwdHh8gISIjJCUmJygpKissLS4vMDEyMzQ1"
      "Njc4"
      "OTo7PD0+"
      "P0BBQkNERUZHSElKS0xNTk9QUVJTVFVWV1hZWltcXV5fYGFiY2RlZmdoaWprbG1ub3Bx"
      "cnN0dXZ3eHl6e3x9fn+"
      "AgYKDhIWGh4iJiouMjY6PkJGSk5SVlpeYmZqbnJ2en6ChoqOkpaanqKmq"
      "q6ytrq+wsbKztLW2t7i5uru8vb6/wMHCw8TFxsfIycrLzM3Oz9DR0tPU1dbX2Nna29zd3t/"
      "g4eLj"
      "5OXm5+jp6uvs7e7v8PHy8/T19vf4+fr7/P3+/"
      "wABAgMEBQYHCAkKCwwNDg8QERITFBUWFxgZGhsc"
      "HR4fICEiIyQlJicoKSorLC0uLzAxMjM0NTY3ODk6Ozw9Pj9AQUJDREVGR0hJSktMTU5PUFFS"
      "U1RV"
      "VldYWVpbXF1eX2BhYmNkZWZnaGlqa2xtbm9wcXJzdHV2d3h5ent8fX5/\r\n"
      "gIGCg4SFhoeIiYqLjI2Oj5CRkpOUlZaXmJmam5ydnp+"
      "goaKjpKWmp6ipqqusra6vsLGys7S1tre4"
      "ubq7vL2+v8DBwsPExcbHyMnKy8zNzs/"
      "Q0dLT1NXW19jZ2tvc3d7f4OHi4+Tl5ufo6err7O3u7/Dx"
      "8vP09fb3+Pn6+/z9/"
      "v8AAQIDBAUGBwgJCgsMDQ4PEBESExQVFhcYGRobHB0eHyAhIiMkJSYnKCkq"
      "KywtLi8wMTIzNDU2Nzg5Ojs8PT4/"
      "QEFCQ0RFRkdISUpLTE1OT1BRUlNUVVZXWFlaW1xdXl9gYWJj"
      "ZGVmZ2hpamtsbW5vcHFyc3R1dnd4eXp7fH1+f4CBgoOEhYaHiImKi4yNjo+"
      "QkZKTlJWWl5iZmpuc"
      "nZ6foKGio6SlpqeoqaqrrK2ur7CxsrO0tba3uLm6u7y9vr/"
      "AwcLDxMXGx8jJysvMzc7P0NHS09TV"
      "1tfY2drb3N3e3+Dh4uPk5ebn6Onq6+zt7u/w8fLz9PX29/j5+vv8/f7/\r\n"
      "AAECAwQFBgcICQoLDA0ODxAREhMUFRYXGBkaGxwdHh8gISIjJCUmJygpKissLS4vMDEyMzQ1"
      "Njc4O"
      "To7PD0+"
      "P0BBQkNERUZHSElKS0xNTk9QUVJTVFVWV1hZWltcXV5fYGFiY2RlZmdoaWprbG1ub3Bxcn"
      "N0dXZ3eHl6e3x9fn+"
      "AgYKDhIWGh4iJiouMjY6PkJGSk5SVlpeYmZqbnJ2en6ChoqOkpaanqKmqq6y"
      "trq+wsbKztLW2t7i5uru8vb6/wMHCw8TFxsfIycrLzM3Oz9DR0tPU1dbX2Nna29zd3t/"
      "g4eLj5OXm"
      "5+jp6uvs7e7v8PHy8/T19vf4+fr7/P3+/"
      "wABAgMEBQYHCAkKCwwNDg8QERITFBUWFxgZGhscHR4fI"
      "CEiIyQlJicoKSorLC0uLzAxMjM0NTY3OD"
      "k6Ozw9Pj9AQUJDREVGR0hJSktMTU5PUFFSU1RVVldYWVpbXF1eX2BhYmNkZWZnaGlqa2xtbm"
      "9wcXJ"
      "zdHV2d3h5ent8fX5/\r\n"
      "gIGCg4SFhoeIiYqLjI2Oj5CRkpOUlZaXmJmam5ydnp+"
      "goaKjpKWmp6ipqqusra6vsLGys7S1tre4ub"
      "q7vL2+v8DBwsPExcbHyMnKy8zNzs/Q0dLT1NXW19jZ2tvc3d7f4OHi4+Tl5ufo6err7O3u7/"
      "Dx8vP0"
      "9fb3+Pn6+/z9/"
      "v8AAQIDBAUGBwgJCgsMDQ4PEBESExQVFhcYGRobHB0eHyAhIiMkJSYnKCkqKywtLi"
      "8wMTIzNDU2Nzg5Ojs8PT4/"
      "QEFCQ0RFRkdISUpLTE1OT1BRUlNUVVZXWFlaW1xdXl9gYWJjZGVmZ2hp"
      "amtsbW5vcHFyc3R1dnd4eXp7fH1+f4CBgoOEhYaHiImKi4yNjo+"
      "QkZKTlJWWl5iZmpucnZ6foKGio6"
      "SlpqeoqaqrrK2ur7CxsrO0tba3uLm6u7y9vr/"
      "AwcLDxMXGx8jJysvMzc7P0NHS09TV1tfY2drb3N3e"
      "3+Dh4uPk5ebn6Onq6+zt7u/w8fLz9PX29/j5+vv8/f7/\r\n"
      "AAECAwQFBgcICQoLDA0ODxAREhMUFRYXGBkaGxwdHh8gISIjJCUmJygpKissLS4vMDEyMzQ1"
      "Njc4OTo"
      "7PD0+"
      "P0BBQkNERUZHSElKS0xNTk9QUVJTVFVWV1hZWltcXV5fYGFiY2RlZmdoaWprbG1ub3BxcnN0"
      "dX"
      "Z3eHl6e3x9fn+"
      "AgYKDhIWGh4iJiouMjY6PkJGSk5SVlpeYmZqbnJ2en6ChoqOkpaanqKmqq6ytrq+ws"
      "bKztLW2t7i5uru8vb6/wMHCw8TFxsfIycrLzM3Oz9DR0tPU1dbX2Nna29zd3t/"
      "g4eLj5OXm5+jp6uvs"
      "7e7v8PHy8/T19vf4+fr7/P3+/"
      "wABAgMEBQYHCAkKCwwNDg8QERITFBUWFxgZGhscHR4fICEiIyQlJic"
      "oKSorLC0uLzAxMjM0NTY3ODk6"
      "Ozw9Pj9AQUJDREVGR0hJSktMTU5PUFFSU1RVVldYWVpbXF1eX2BhYmNkZWZnaGlqa2xtbm9w"
      "cXJzdHV"
      "2d3h5ent8fX5/\r\n"
      "gIGCg4SFhoeIiYqLjI2Oj5CRkpOUlZaXmJmam5ydnp+"
      "goaKjpKWmp6ipqqusra6vsLGys7S1tre4ubq7"
      "vL2+v8DBwsPExcbHyMnKy8zNzs/Q0dLT1NXW19jZ2tvc3d7f4OHi4+Tl5ufo6err7O3u7/"
      "Dx8vP09fb3"
      "+Pn6+/z9/v8=\r\n"
      "\r\n" +
      smtp::Mime::kBoundary + "\r\n";

  REQUIRE(expected == actual);
}
