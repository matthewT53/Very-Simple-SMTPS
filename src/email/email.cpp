#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <string>

#include "email/email.hpp"
#include "mime/mime.hpp"

#include "curl/curl.h"

namespace smtp {

struct UploadStatus {
  uint64_t lines_read;
  std::vector<std::string> email_contents;
};

static size_t payloadCallback(void *ptr, size_t size, size_t nmemb, void *userp);
static std::string getCurrentDateTime();

Email::Email(const EmailParams &params)
    : m_smtp_user{params.user}, m_smtp_password{params.password},
      m_smtp_host{params.hostname}, m_date{getCurrentDateTime()} {}

void Email::addAttachment(const Attachment &attachment) { m_attachments.push_back(attachment); }

void Email::removeAttachment(std::string_view file_path) {
  const auto &checkFilePath = [&file_path](const Attachment &attachment) {
    return attachment.getFilePath() == file_path;
  };
  if (const auto &it = std::find_if(m_attachments.begin(), m_attachments.end(), checkFilePath);
      it != m_attachments.end()) {
    m_attachments.erase(it);
  }
}

static std::string getCurrentDateTime() {
  auto cur_time = std::chrono::system_clock::now();
  const std::time_t &time_t_obj = std::chrono::system_clock::to_time_t(cur_time);

  std::stringstream ss;
  ss << std::put_time(std::localtime(&time_t_obj), "%d/%m/%Y %I:%M:%S +1100");

  return ss.str();
}

std::vector<std::string> Email::build() const {
  std::vector<std::string> result;

  result.push_back("To: " + m_to + "\r\n");
  result.push_back("From: " + m_from + "\r\n");
  result.push_back("Cc: " + m_cc + "\r\n");
  result.push_back("Subject: " + m_subject + "\r\n");
  result.push_back(m_date + "\r\n");

  smtp::Mime m_mime;
  m_mime.addMessage(m_body);

  for (const auto &attachment : m_attachments) {
    m_mime.addAttachment(attachment.getFilePath(), attachment.getContentsAsB64());
  }

  const std::vector<std::string> &mime_lines = m_mime.build();
  for (const auto &line : mime_lines) {
    result.push_back(line);
  }

  result.push_back(smtp::Mime::kLastBoundary);
  result.emplace_back("\r\n.\r\n");

  return result;
}

void Email::send() const {
  CURL *curl = nullptr;
  CURLcode res = CURLE_OK;
  struct curl_slist *recipients = nullptr;
  UploadStatus upload_ctx;

  upload_ctx.email_contents = build();
  upload_ctx.lines_read = 0;

  curl = curl_easy_init();

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_USERNAME, m_smtp_user.c_str());
    curl_easy_setopt(curl, CURLOPT_PASSWORD, m_smtp_password.c_str());

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER,
                     0); // allows emails to be sent
    curl_easy_setopt(curl, CURLOPT_URL, m_smtp_host.c_str());

    /* If you want to connect to a site who isn't using a certificate that is
     * signed by one of the certs in the CA bundle you have, you can skip the
     * verification of the server's certificate. This makes the connection
     * A LOT LESS SECURE.
     *
     * If you have a CA cert for the server stored someplace else than in the
     * default bundle, then the CURLOPT_CAPATH option might come handy for
     * you. */
#ifdef SKIP_PEER_VERIFICATION
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
#endif

    /* If the site you're connecting to uses a different host name that what
     * they have mentioned in their server certificate's commonName (or
     * subjectAltName) fields, libcurl will refuse to connect. You can skip
     * this check, but this will make the connection less secure. */
#ifdef SKIP_HOSTNAME_VERIFICATION
    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
#endif

    /* Note that this option isn't strictly required, omitting it will result
     * in libcurl sending the MAIL FROM command with empty sender data. All
     * autoresponses should have an empty reverse-path, and should be directed
     * to the address in the reverse-path which triggered them. Otherwise,
     * they could cause an endless loop. See RFC 5321 Section 4.5.5 for more
     * details.
     */
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, m_from.c_str());

    /* Add two recipients, in this particular case they correspond to the
     * To: and Cc: addressees in the header, but they could be any kind of
     * recipient. */
    recipients = curl_slist_append(recipients, m_to.c_str());
    if (m_cc.length() > 0) {
      recipients = curl_slist_append(recipients, m_cc.c_str());
    }

    curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);

    /* We're using a callback function to specify the payload (the headers and
     * body of the message). You could just use the CURLOPT_READDATA option to
     * specify a FILE pointer to read from. */
    curl_easy_setopt(curl, CURLOPT_READFUNCTION, payloadCallback);
    curl_easy_setopt(curl, CURLOPT_READDATA, &upload_ctx);
    curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

    /* Since the traffic will be encrypted, it is very useful to turn on debug
     * information within libcurl to see what is happening during the
     * transfer */
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

    /* Send the message */
    res = curl_easy_perform(curl);

    /* Check for errors */
    if (res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    /* Free the list of recipients */
    curl_slist_free_all(recipients);

    /* Always cleanup */
    curl_easy_cleanup(curl);
  }
}

static size_t payloadCallback(void *ptr, size_t size, size_t nmemb, void *userp) {
  auto *upload_ctx = static_cast<UploadStatus *>(userp);
  const char *data = nullptr;

  // No more data to send
  if ((size == 0) || (nmemb == 0) || ((size * nmemb) < 1)) {
    return 0;
  }

  if (upload_ctx->lines_read < upload_ctx->email_contents.size()) {
    data = upload_ctx->email_contents[upload_ctx->lines_read].c_str();

    if (data) {
      size_t len = strlen(data);
      memcpy(ptr, data, len);
      upload_ctx->lines_read++;

      return len;
    }
  }

  return 0;
}

void Email::clear() {
  m_smtp_user.clear();
  m_smtp_password.clear();
  m_smtp_host.clear();

  m_to.clear();
  m_from.clear();
  m_cc.clear();
  m_subject.clear();
  m_date = getCurrentDateTime();
  m_body.clear();

  m_attachments.clear();
}

} // namespace smtp
