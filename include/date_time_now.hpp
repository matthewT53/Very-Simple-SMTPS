#pragma once

#include "date_time.hpp"

namespace smtp {

class DateTimeNow : public DateTime {
public:
  std::string getTimestamp() const override;
};

} // namespace smtp
