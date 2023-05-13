#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>

#include "base64.hpp"
#include "mime.hpp"

namespace smtp {

const std::string Mime::kBoundaryDeclare = "----------030203080101020302070708";
const std::string Mime::kBoundary = "--" + kBoundaryDeclare;
const std::string Mime::kLastBoundary = kBoundary + "--";
const std::string Mime::kCRLF = "\r\n";

// The base64 contents of an attachment are split into chunks of 512 bytes.
const int kTransferRate = 512;

Mime::Mime(const std::string &p_user_agent) : m_user_agent{p_user_agent} {}

std::vector<std::string> Mime::build() const {
  std::vector<std::string> result;

  if (m_messages.size() > 0 || m_files.size() > 0) {
    buildHeader(result);
    buildMessages(result);
    buildAttachments(result);
  }

  return result;
}

void Mime::buildHeader(std::vector<std::string> &p_contents) const {
  p_contents.push_back("User-Agent: " + m_user_agent + "\r\n");
  p_contents.push_back("MIME-Version: 1.0\r\n");
  p_contents.push_back("Content-Type: multipart/mixed;\r\n");
  p_contents.push_back(" boundary=\"----------030203080101020302070708\"\r\n");
  p_contents.push_back("\r\nThis is a multi-part message in MIME format.\r\n");
  p_contents.push_back(Mime::kBoundary);
  p_contents.push_back("\r\n");
}

void Mime::buildMessages(std::vector<std::string> &p_contents) const {
  for (const std::string &message : m_messages) {
    p_contents.push_back("Content-Type: text/plain; charset=utf-8; format=flowed\r\n");
    p_contents.push_back("Content-Transfer-Encoding: 7bit\r\n");
    p_contents.push_back("\r\n");
    p_contents.push_back(message + "\r\n");
    p_contents.push_back(Mime::kBoundary + "\r\n");
  }
}

void Mime::buildAttachments(std::vector<std::string> &p_contents) const {
  for (const std::string &attachment : m_files) {
    std::ifstream ifs(attachment, std::ifstream::binary);
    if (!ifs) {
      throw std::runtime_error("[!] Failed to open file: " + attachment);
    }

    if (!ifs.good()) {
      throw std::runtime_error("[!] Input stream for file: " + attachment +
                               " is not in a good state!");
    }

    // TODO: Find a better method since this is really slow!
    std::stringstream ss;
    ss << ifs.rdbuf();
    ifs.close();

    // write content to stdout
    const std::string &contents = ss.str();
    const std::vector<byte> content_bytes(contents.begin(), contents.end());
    const std::string &contents_b64 = Base64::Base64Encode(content_bytes);
    const std::string &filename = std::filesystem::path(attachment).filename().string();

    p_contents.push_back("Content-Type: application/octet-stream\r\n");
    p_contents.push_back("Content-Transfer-Encoding: base64\r\n");
    p_contents.push_back("Content-Disposition: attachment;\r\n");
    p_contents.push_back(" filename=" + filename + "\r\n");
    p_contents.push_back("\r\n");

    // Split the base64 encoded contents into chunks of 512 bytes
    uint32_t bytes_read = 0;
    while (bytes_read < contents_b64.size()) {
      const std::string &data = contents_b64.substr(bytes_read, kTransferRate);
      bytes_read += data.size();

      p_contents.push_back(data + "\r\n");
    }

    p_contents.push_back("\r\n");
    p_contents.push_back(Mime::kBoundary);
    p_contents.push_back("\r\n");
  }
}

void Mime::addAttachment(const std::string &p_attachment) { m_files.push_back(p_attachment); }

void Mime::addMessage(const std::string &p_message) { m_messages.push_back(p_message); }

void Mime::removeAttachment(const std::string &p_filename) {
  auto it = std::find(m_files.begin(), m_files.end(), p_filename);
  if (it != m_files.end()) {
    m_files.erase(it);
  }
}

void Mime::removeMessage(const std::string &t_message) {
  auto it = std::find(m_messages.begin(), m_messages.end(), t_message);
  if (it != m_messages.end()) {
    m_messages.erase(it);
  }
}

std::ostream &Mime::output(std::ostream &p_out) const {
  std::string contents;
  for (const std::string &line : build())
    contents += line;
  return p_out << contents;
}

} // namespace smtp
