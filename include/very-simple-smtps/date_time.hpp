#pragma once

#include <string>

namespace smtp {

class DateTime {
public:
  virtual ~DateTime() = default;
  virtual std::string getTimestamp() const = 0;
};

} // namespace smtp
