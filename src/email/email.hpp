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
};

class Email {
public:
  explicit Email(const EmailParams &params);

  void setTo(std::string_view to) { m_to = to; }
  void setFrom(std::string_view from) { m_from = from; }
  void setCc(std::string_view cc) { m_cc = cc; }
  void setSubject(std::string_view subject) { m_subject = subject; }
  void setBody(std::string_view body) { m_body = body; }
  void setDate(const DateTime &dt) { m_date = dt.getTimestamp(); }

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
  std::string m_date;
  std::string m_body;

  std::vector<Attachment> m_attachments;

  std::vector<std::string> build() const;

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
