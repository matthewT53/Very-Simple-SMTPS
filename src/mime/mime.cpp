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

Mime::Mime(const std::string &user_agent) : m_user_agent{user_agent} { buildHeader(); }

void Mime::buildHeader() {
  m_document.push_back("User-Agent: " + m_user_agent + "\r\n");
  m_document.emplace_back("MIME-Version: 1.0\r\n");
  m_document.emplace_back("Content-Type: multipart/mixed;\r\n");
  m_document.emplace_back(" boundary=\"----------030203080101020302070708\"\r\n");
  m_document.emplace_back("\r\nThis is a multi-part message in MIME format.\r\n");
  m_document.push_back(Mime::kBoundary);
  m_document.emplace_back("\r\n");
}

void Mime::addMessage(const std::string &message) {
  m_document.emplace_back("Content-Type: text/plain; charset=utf-8; format=flowed\r\n");
  m_document.emplace_back("Content-Transfer-Encoding: 7bit\r\n");
  m_document.emplace_back("\r\n");
  m_document.push_back(message + "\r\n");
  m_document.push_back(Mime::kBoundary + "\r\n");
}

void Mime::addAttachment(const std::string &attachment_path, const std::string &contents_b64) {
  const std::string &filename = std::filesystem::path(attachment_path).filename().string();

  m_document.emplace_back("Content-Type: application/octet-stream\r\n");
  m_document.emplace_back("Content-Transfer-Encoding: base64\r\n");
  m_document.emplace_back("Content-Disposition: attachment;\r\n");
  m_document.push_back(" filename=" + filename + "\r\n");
  m_document.emplace_back("\r\n");

  // Split the base64 encoded contents into chunks of 512 bytes
  uint32_t bytes_read = 0;
  while (bytes_read < contents_b64.size()) {
    const std::string &data = contents_b64.substr(bytes_read, kTransferRate);
    bytes_read += data.size();

    m_document.push_back(data + "\r\n");
  }

  m_document.emplace_back("\r\n");
  m_document.push_back(Mime::kBoundary);
  m_document.emplace_back("\r\n");
}

std::ostream &Mime::output(std::ostream &out) const {
  std::string contents;
  for (const std::string &line : build()) {
    contents += line;
  }
  return out << contents;
}

} // namespace smtp
