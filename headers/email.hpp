/*
    Code to send an email with attachments using LIBCURL
    Supports SMTPS only.
*/

#pragma once

#include <memory>
#include <string>
#include <vector>

#include "mime.hpp"
#include "secure_strings.hpp"

namespace smtp {

class Email {
public:
  Email();

  inline void setTo(const std::string &p_to) { m_to = p_to; }
  inline void setFrom(const std::string &p_from) { m_from = p_from; }
  inline void setCc(const std::string &p_cc) { m_cc = p_cc; }
  inline void setSubject(const std::string &p_subject) {
    m_subject = p_subject;
  }
  inline void setBody(const std::string &p_body) {
    m_body = p_body;
    m_mime->addMessage(m_body);
  }

  inline void setSmtpUsername(const std::string &p_user) {
    m_smtp_user = p_user;
  }
  inline void setSmtpPassword(const std::string &p_password) {
    m_smtp_password = p_password;
  }
  inline void setSmtpHost(const std::string &p_hostname) {
    m_smtp_host = p_hostname;
  }

  inline void addAttachment(const std::string &p_attachment) {
    m_mime->addAttachment(p_attachment);
  }
  inline void removeAttachment(const std::string &p_attachment) {
    m_mime->removeAttachment(p_attachment);
  }
  inline void setMimeDocument(std::unique_ptr<smtp::IMime> &p_mime) {
    m_mime = std::move(p_mime);
  }

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
  static std::size_t payloadCallback(void *ptr, size_t size, size_t nmemb,
                                     void *userp);

  friend std::ostream &operator<<(std::ostream &p_out, Email &p_email) {
    const std::vector<std::string> &email_contents = p_email.build();
    std::string result;
    for (const auto &line : email_contents)
      result += line;
    return p_out << result;
  }
};

} // namespace smtp
