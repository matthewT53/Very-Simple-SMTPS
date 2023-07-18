#pragma once

#include <exception>
#include <iostream>
#include <string>
#include <vector>

namespace smtp {

class MimeAttachmentException : public std::runtime_error {
public:
  using runtime_error::runtime_error;
};

class Mime {
public:
  explicit Mime(const std::string &user_agent = "Very-Simple-SMTPS");

  void addAttachment(const std::string &attachment_path, const std::string &contents_b64);
  void addMessage(const std::string &message);

  std::vector<std::string> build() const { return m_document; }

  static const std::string kBoundaryDeclare;
  static const std::string kBoundary;
  static const std::string kLastBoundary;
  static const std::string kCRLF;

protected:
  std::ostream &output(std::ostream &out) const;

private:
  std::vector<std::string> m_document;
  std::string m_user_agent;

  void buildHeader();
};

} // namespace smtp
