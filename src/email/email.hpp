/*
    Code to send an email with attachments using LIBCURL
    Supports SMTPS only.
*/

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "mime/mime.hpp"
#include "utils/secure_strings.hpp"

namespace smtp {

class Email {
public:
  Email(const std::string &user, const std::string &password, const std::string &hostname);

  void setTo(const std::string &p_to) { m_to = p_to; }
  void setFrom(const std::string &p_from) { m_from = p_from; }
  void setCc(const std::string &p_cc) { m_cc = p_cc; }
  void setSubject(const std::string &p_subject) { m_subject = p_subject; }
  void setBody(const std::string &p_body) {
    m_body = p_body;
    m_mime->addMessage(m_body);
  }

  void addAttachment(const std::string &p_attachment) { m_mime->addAttachment(p_attachment); }
  void removeAttachment(const std::string &p_attachment) { m_mime->removeAttachment(p_attachment); }
  void setMimeDocument(std::unique_ptr<smtp::IMime> &p_mime) { m_mime = std::move(p_mime); }

  void clear();
  void send() const;

private:
  // smtp information
  smtp::secure_string m_smtp_user;
  smtp::secure_string m_smtp_password;
  smtp::secure_string m_smtp_host;

  // email data
  std::string m_to;
  std::string m_from;
  std::string m_cc;
  std::string m_subject;
  std::string m_body;

  // Mime document with possible attachments
  std::unique_ptr<smtp::IMime> m_mime;

  std::vector<std::string> build() const;

  friend std::ostream &operator<<(std::ostream &p_out, Email &p_email) {
    const std::vector<std::string> &email_contents = p_email.build();
    std::string result;
    for (const auto &line : email_contents)
      result += line;
    return p_out << result;
  }
};

} // namespace smtp
