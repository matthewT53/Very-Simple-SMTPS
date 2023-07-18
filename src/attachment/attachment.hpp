#pragma once

#include <exception>
#include <iostream>
#include <string>
#include <vector>

namespace smtp {

class AttachmentException : public std::runtime_error {
public:
  using runtime_error::runtime_error;
};

class Attachment {
public:
  Attachment() = default;
  explicit Attachment(const std::string &file_path);

  std::string getFilePath() const { return m_file_path; }

  std::string getContentsAsB64() const;
  void setContents(const std::vector<uint8_t> &contents);

  bool operator==(const Attachment &other) const { return m_file_path == other.m_file_path; }

private:
  std::vector<uint8_t> m_contents;
  std::string m_file_path;
};

} // namespace smtp
