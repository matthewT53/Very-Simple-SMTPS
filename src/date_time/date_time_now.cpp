#include "date_time_now.hpp"

#include <chrono>
#include <ctime>
#include <iomanip>

namespace smtp {

std::string DateTimeNow::getTimestamp() const {
  auto cur_time = std::chrono::system_clock::now();
  const std::time_t &time_t_obj = std::chrono::system_clock::to_time_t(cur_time);

  std::stringstream ss;
  ss << std::put_time(std::localtime(&time_t_obj), "%d/%m/%Y %I:%M:%S +1100");

  return ss.str();
}

} // namespace smtp
