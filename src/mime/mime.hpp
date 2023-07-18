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
  explicit Mime(const std::string &user_agent = "test bot");

  void addAttachment(const std::string &filename);
  void removeAttachment(const std::string &filename);
  void addMessage(const std::string &message);
  void removeMessage(const std::string &message);

  std::vector<std::string> build() const;

  static const std::string kBoundaryDeclare;
  static const std::string kBoundary;
  static const std::string kLastBoundary;
  static const std::string kCRLF;

protected:
  std::ostream &output(std::ostream &out) const;

private:
  std::vector<std::string> m_files;
  std::vector<std::string> m_messages;

  std::string m_user_agent;

  void buildHeader(std::vector<std::string> &contents) const;
  void buildMessages(std::vector<std::string> &contents) const;
  void buildAttachments(std::vector<std::string> &contents) const;
};

} // namespace smtp
