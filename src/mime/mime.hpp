#pragma once

#include <iostream>
#include <string>
#include <vector>

namespace smtp {

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

private:
  std::vector<std::string> m_document;
  std::string m_user_agent;

  void buildHeader();

  std::ostream &output(std::ostream &out) const {
    std::string contents;
    for (const std::string &line : build()) {
      contents += line;
    }
    return out << contents;
  }

  friend std::ostream &operator<<(std::ostream &p_out, const Mime &p_mime) {
    return p_mime.output(p_out);
  }
};

} // namespace smtp
