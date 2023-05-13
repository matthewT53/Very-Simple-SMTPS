#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "mime_abstract.hpp"

namespace smtp {

class Mime : public smtp::IMime {
public:
  Mime(const std::string &p_user_agent = "test bot");

  // Adds a file specified by t_filename as an attachment to this mime message
  void addAttachment(const std::string &p_filename) override;

  // Removes the attachment specified by filename from this Mime document
  void removeAttachment(const std::string &p_filename) override;

  // Adds a body to the email address
  void addMessage(const std::string &p_message) override;

  // Removes the body message specified by the message contents from this Mime
  // document
  void removeMessage(const std::string &p_message) override;

  // Constructs the mime message with the version, user agent as well as any
  // attachments
  std::vector<std::string> build() const override;

  static const std::string kBoundaryDeclare;
  static const std::string kBoundary;
  static const std::string kLastBoundary;
  static const std::string kCRLF;

protected:
  std::ostream &output(std::ostream &p_out) const override;

private:
  std::vector<std::string> m_files;
  std::vector<std::string> m_messages;

  std::string m_user_agent;

  void buildHeader(std::vector<std::string> &p_contents) const;
  void buildMessages(std::vector<std::string> &p_contents) const;
  void buildAttachments(std::vector<std::string> &p_contents) const;
};

} // namespace smtp
