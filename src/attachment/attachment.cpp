#include "attachment.hpp"
#include "utils/base64/base64.hpp"

#include <algorithm>
#include <fstream>
#include <iostream>
#include <sstream>

namespace smtp {

Attachment::Attachment(const std::string &file_path) : m_file_path{file_path} {
  std::ifstream ifs(file_path, std::ifstream::binary);
  // TODO: Find a better method since this is really slow!
  std::stringstream ss;
  ss << ifs.rdbuf();
  if (!ifs) {
    throw AttachmentException("[!] Failed to open file: " + file_path);
  }

  ifs.close();

  const std::string &file_contents = ss.str();
  m_contents = std::vector<uint8_t>(file_contents.begin(), file_contents.end());
}

std::string Attachment::getContentsAsB64() const { return Base64::Base64Encode(m_contents); }

void Attachment::setContents(const std::vector<uint8_t> &contents) { m_contents = contents; }

} // namespace smtp
