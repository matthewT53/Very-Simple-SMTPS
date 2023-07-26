#pragma once

#include <cstdint>
#include <exception>
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace smtp {

class AttachmentException : public std::runtime_error {
public:
  using runtime_error::runtime_error;
};

class Attachment {
public:
  Attachment() = default;
  // This constructor will open the file at file_path in read only mode and will then
  // read the whole file into m_contents
  explicit Attachment(const std::string &file_path);

  std::string getFilePath() const { return m_file_path; }
  void setFilePath(std::string_view file_path) { m_file_path = file_path; }

  // Returns the base64 encoded contents
  std::string getContentsAsB64() const;
  void setContents(const std::vector<uint8_t> &contents);

private:
  std::vector<uint8_t> m_contents;
  std::string m_file_path;
};

} // namespace smtp
