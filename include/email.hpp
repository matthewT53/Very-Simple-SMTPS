#pragma once

#include <string>
#include <string_view>
#include <vector>

#include "attachment/attachment.hpp"
#include "date_time/date_time_now.hpp"
#include "utils/secure_strings.hpp"

namespace smtp {

struct EmailParams {
  std::string_view user;
  std::string_view password;
  std::string_view hostname;

  std::string_view to;
  std::string_view from;
  std::string_view cc;
  std::string_view subject;
  std::string_view body;
  const DateTime *datetime = nullptr;
};

class Email {
public:
  explicit Email(const EmailParams &params);

  void addAttachment(const Attachment &attachment);
  void removeAttachment(std::string_view file_path);

  void clear();
  void send() const;

private:
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

  std::vector<std::string> build() const;
  std::string getDatetime() const;

  friend std::ostream &operator<<(std::ostream &out, const Email &email) {
    const std::vector<std::string> &email_contents = email.build();
    std::string result;
    for (const auto &line : email_contents) {
      result += line;
    }
    return out << result;
  }
};

} // namespace smtp
