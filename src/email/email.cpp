#include <algorithm>
#include <cstdint>
#include <string>

#include "date_time/date_time_now.hpp"
#include "email/email.hpp"
#include "mime/mime.hpp"
#include "utils/secure_strings.hpp"

#include "curl/curl.h"

namespace smtp {

struct UploadStatus {
  uint64_t lines_read;
  std::vector<std::string> email_contents;
};

struct Email::Impl {
  // smtp information
  smtp::secure_string m_smtp_user;
  smtp::secure_string m_smtp_password;
  smtp::secure_string m_smtp_host;

  // email data
  std::string m_to;
  std::string m_from;
  std::string m_cc;
  std::string m_subject;
  std::string m_body;

  const DateTime *m_date = nullptr;
  std::vector<Attachment> m_attachments;
};

static size_t payloadCallback(void *ptr, size_t size, size_t nmemb, void *userp);

Email::Email(const EmailParams &params) : m_impl{std::make_unique<Impl>()} {
  m_impl->m_smtp_user = params.user;
  m_impl->m_smtp_password = params.password;
  m_impl->m_smtp_host = params.hostname;
  m_impl->m_to = params.to;
  m_impl->m_from = params.from;
  m_impl->m_cc = params.cc;
  m_impl->m_subject = params.subject;
  m_impl->m_body = params.body;
  m_impl->m_date = params.datetime;
}

Email::~Email() = default;

void Email::addAttachment(const Attachment &attachment) {
  m_impl->m_attachments.push_back(attachment);
}

void Email::removeAttachment(std::string_view file_path) {
  const auto &checkFilePath = [&file_path](const Attachment &attachment) {
    return attachment.getFilePath() == file_path;
  };

  if (const auto &it =
          std::find_if(m_impl->m_attachments.begin(), m_impl->m_attachments.end(), checkFilePath);
      it != m_impl->m_attachments.end()) {
    m_impl->m_attachments.erase(it);
  }
}

std::vector<std::string> Email::build() const {
  std::vector<std::string> result;

  result.push_back("To: " + m_impl->m_to + "\r\n");
  result.push_back("From: " + m_impl->m_from + "\r\n");
  result.push_back("Cc: " + m_impl->m_cc + "\r\n");
  result.push_back("Subject: " + m_impl->m_subject + "\r\n");
  result.push_back(this->getDatetime() + "\r\n");

  smtp::Mime m_mime;
  m_mime.addMessage(m_impl->m_body);

  for (const auto &attachment : m_impl->m_attachments) {
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

  upload_ctx.email_contents = this->build();
  upload_ctx.lines_read = 0;

  curl = curl_easy_init();

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_USERNAME, m_impl->m_smtp_user.c_str());
    curl_easy_setopt(curl, CURLOPT_PASSWORD, m_impl->m_smtp_password.c_str());

    curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER,
                     0); // allows emails to be sent
    curl_easy_setopt(curl, CURLOPT_URL, m_impl->m_smtp_host.c_str());

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
    curl_easy_setopt(curl, CURLOPT_MAIL_FROM, m_impl->m_from.c_str());

    /* Add two recipients, in this particular case they correspond to the
     * To: and Cc: addressees in the header, but they could be any kind of
     * recipient. */
    recipients = curl_slist_append(recipients, m_impl->m_to.c_str());
    if (m_impl->m_cc.length() > 0) {
      recipients = curl_slist_append(recipients, m_impl->m_cc.c_str());
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

std::string Email::getDatetime() const {
  const auto &datetimenow = std::make_unique<DateTimeNow>();
  return m_impl->m_date ? m_impl->m_date->getTimestamp() : datetimenow->getTimestamp();
}

void Email::clear() {
  m_impl->m_smtp_user.clear();
  m_impl->m_smtp_password.clear();
  m_impl->m_smtp_host.clear();

  m_impl->m_to.clear();
  m_impl->m_from.clear();
  m_impl->m_cc.clear();
  m_impl->m_subject.clear();
  m_impl->m_body.clear();

  m_impl->m_attachments.clear();
}

} // namespace smtp
