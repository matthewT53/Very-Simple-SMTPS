#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "mime/mime.hpp"
#include "utils/base64/base64.hpp"

namespace smtp {

const std::string Mime::kBoundaryDeclare = "----------030203080101020302070708";
const std::string Mime::kBoundary = "--" + kBoundaryDeclare;
const std::string Mime::kLastBoundary = kBoundary + "--";
const std::string Mime::kCRLF = "\r\n";

// The base64 contents of an attachment are split into chunks of 512 bytes.
const int kTransferRate = 512;

// Removes an item from a vector if it exists otherwise the vector is unchanged
static void removeItem(std::vector<std::string> &vec, const std::string &item);

Mime::Mime(const std::string &user_agent) : m_user_agent{user_agent} {}

std::vector<std::string> Mime::build() const {
  std::vector<std::string> result;

  if (!m_messages.empty() || !m_files.empty()) {
    buildHeader(result);
    buildMessages(result);
    buildAttachments(result);
  }

  return result;
}

void Mime::buildHeader(std::vector<std::string> &contents) const {
  contents.push_back("User-Agent: " + m_user_agent + "\r\n");
  contents.emplace_back("MIME-Version: 1.0\r\n");
  contents.emplace_back("Content-Type: multipart/mixed;\r\n");
  contents.emplace_back(" boundary=\"----------030203080101020302070708\"\r\n");
  contents.emplace_back("\r\nThis is a multi-part message in MIME format.\r\n");
  contents.push_back(Mime::kBoundary);
  contents.emplace_back("\r\n");
}

void Mime::buildMessages(std::vector<std::string> &contents) const {
  for (const std::string &message : m_messages) {
    contents.emplace_back("Content-Type: text/plain; charset=utf-8; format=flowed\r\n");
    contents.emplace_back("Content-Transfer-Encoding: 7bit\r\n");
    contents.emplace_back("\r\n");
    contents.push_back(message + "\r\n");
    contents.push_back(Mime::kBoundary + "\r\n");
  }
}

void Mime::buildAttachments(std::vector<std::string> &contents) const {
  for (const std::string &attachment : m_files) {
    std::ifstream ifs(attachment, std::ifstream::binary);
    // TODO: Find a better method since this is really slow!
    std::stringstream ss;
    ss << ifs.rdbuf();
    if (!ifs) {
      throw MimeAttachmentException("[!] Failed to open file: " + attachment);
    }

    ifs.close();

    const std::string &file_contents = ss.str();
    const std::vector<byte> content_bytes(file_contents.begin(), file_contents.end());
    const std::string &contents_b64 = Base64::Base64Encode(content_bytes);
    const std::string &filename = std::filesystem::path(attachment).filename().string();

    contents.emplace_back("Content-Type: application/octet-stream\r\n");
    contents.emplace_back("Content-Transfer-Encoding: base64\r\n");
    contents.emplace_back("Content-Disposition: attachment;\r\n");
    contents.push_back(" filename=" + filename + "\r\n");
    contents.emplace_back("\r\n");

    // Split the base64 encoded contents into chunks of 512 bytes
    uint32_t bytes_read = 0;
    while (bytes_read < contents_b64.size()) {
      const std::string &data = contents_b64.substr(bytes_read, kTransferRate);
      bytes_read += data.size();

      contents.push_back(data + "\r\n");
    }

    contents.emplace_back("\r\n");
    contents.push_back(Mime::kBoundary);
    contents.emplace_back("\r\n");
  }
}

void Mime::addAttachment(const std::string &p_attachment) { m_files.push_back(p_attachment); }

void Mime::addMessage(const std::string &p_message) { m_messages.push_back(p_message); }

void Mime::removeAttachment(const std::string &filename) { removeItem(m_files, filename); }

void Mime::removeMessage(const std::string &message) { removeItem(m_messages, message); }

static void removeItem(std::vector<std::string> &vec, const std::string &item) {
  auto it = std::find(vec.begin(), vec.end(), item);
  if (it != vec.end()) {
    vec.erase(it);
  }
}

std::ostream &Mime::output(std::ostream &out) const {
  std::string contents;
  for (const std::string &line : build()) {
    contents += line;
  }
  return out << contents;
}

} // namespace smtp
