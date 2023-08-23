#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <vector>

#include "attachment.hpp"
#include "date_time.hpp"

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

  ~Email();

  void addAttachment(const Attachment &attachment);
  void removeAttachment(std::string_view file_path);

  void clear();
  void send() const;

private:
  struct Impl;
  std::unique_ptr<Impl> m_impl;

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
