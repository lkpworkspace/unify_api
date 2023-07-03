/****************************************************************************
Copyright (c) 2023, likepeng
All rights reserved.

Author: likepeng <likepeng0418@163.com>
****************************************************************************/
#include "time/time.h"

#include <ctime>
#include <chrono>
#include <iomanip>
#include <limits>
#include <sstream>
#include <thread>

namespace unify_api {

using std::chrono::high_resolution_clock;
using std::chrono::steady_clock;
using std::chrono::system_clock;

const Time Time::MAX = Time(std::numeric_limits<int64_t>::max());
const Time Time::MIN = Time(1);

Time::Time(int64_t nanoseconds) { nanoseconds_ = nanoseconds; }

Time::Time(int nanoseconds) {
  nanoseconds_ = static_cast<int64_t>(nanoseconds);
}

Time::Time(double seconds) {
  nanoseconds_ = static_cast<int64_t>(seconds * 1000000000UL);
}

Time::Time(uint32_t seconds, uint32_t nanoseconds) {
  nanoseconds_ = static_cast<int64_t>(seconds) * 1000000000UL + nanoseconds;
}

Time::Time(const Time& other) { nanoseconds_ = other.nanoseconds_; }

Time& Time::operator=(const Time& other) {
  this->nanoseconds_ = other.nanoseconds_;
  return *this;
}

Time Time::Now() {
  auto now = system_clock::now();
  auto nano_time_point =
      std::chrono::time_point_cast<std::chrono::nanoseconds>(now);
  auto epoch = nano_time_point.time_since_epoch();
  int64_t now_nano =
      std::chrono::duration_cast<std::chrono::nanoseconds>(epoch).count();
  return Time(now_nano);
}

Time Time::MonoTime() {
  auto now = steady_clock::now();
  auto nano_time_point =
      std::chrono::time_point_cast<std::chrono::nanoseconds>(now);
  auto epoch = nano_time_point.time_since_epoch();
  int64_t now_nano =
      std::chrono::duration_cast<std::chrono::nanoseconds>(epoch).count();
  return Time(now_nano);
}

double Time::ToSecond() const {
  return static_cast<double>(nanoseconds_) / 1000000000UL;
}

bool Time::IsZero() const { return nanoseconds_ == 0; }

int64_t Time::ToNanosecond() const { return nanoseconds_; }

std::string Time::ToString() const {
#if defined(__APPLE__)
  auto micro = std::chrono::microseconds(nanoseconds_ / 1000);
  std::chrono::time_point<system_clock, std::chrono::microseconds> tp(micro);
#elif defined(_WIN32) && defined(_WINDOWS)
  auto nano = std::chrono::nanoseconds(nanoseconds_);
  std::chrono::system_clock::time_point tp(nano);
#else
  auto nano = std::chrono::nanoseconds(nanoseconds_);
  std::chrono::time_point<system_clock, std::chrono::nanoseconds> tp(nano);
#endif
  auto time = system_clock::to_time_t(tp);
  struct tm stm;
#if defined(_WIN32) && defined(_WINDOWS)
  auto ret = localtime_s(&time, &stm);
#else
  auto ret = localtime_r(&time, &stm);
#endif
  if (ret == nullptr) {
    return std::to_string(static_cast<double>(nanoseconds_) / 1000000000.0);
  }

  std::stringstream ss;
#if __GNUC__ >= 5
  ss << std::put_time(ret, "%F %T");
  ss << "." << std::setw(9) << std::setfill('0')
    << nanoseconds_ % 1000000000UL;
#else
  char date_time[128];
  strftime(date_time, sizeof(date_time), "%F %T", ret);
  ss << std::string(date_time) << "." << std::setw(9) << std::setfill('0')
     << nanoseconds_ % 1000000000UL;
#endif
  return ss.str();
}

void Time::SleepUntil(const Time& time) {
  auto nano = std::chrono::nanoseconds(time.ToNanosecond());
  std::chrono::time_point<system_clock, std::chrono::nanoseconds> tp(nano);
  std::this_thread::sleep_until(tp);
}

Duration Time::operator-(const Time& rhs) const {
  return Duration(static_cast<int64_t>(nanoseconds_ - rhs.nanoseconds_));
}

Time Time::operator+(const Duration& rhs) const {
  return Time(nanoseconds_ + rhs.ToNanosecond());
}

Time Time::operator-(const Duration& rhs) const {
  return Time(nanoseconds_ - rhs.ToNanosecond());
}

Time& Time::operator+=(const Duration& rhs) {
  *this = *this + rhs;
  return *this;
}

Time& Time::operator-=(const Duration& rhs) {
  *this = *this - rhs;
  return *this;
}

bool Time::operator==(const Time& rhs) const {
  return nanoseconds_ == rhs.nanoseconds_;
}

bool Time::operator!=(const Time& rhs) const {
  return nanoseconds_ != rhs.nanoseconds_;
}

bool Time::operator>(const Time& rhs) const {
  return nanoseconds_ > rhs.nanoseconds_;
}

bool Time::operator<(const Time& rhs) const {
  return nanoseconds_ < rhs.nanoseconds_;
}

bool Time::operator>=(const Time& rhs) const {
  return nanoseconds_ >= rhs.nanoseconds_;
}

bool Time::operator<=(const Time& rhs) const {
  return nanoseconds_ <= rhs.nanoseconds_;
}

std::ostream& operator<<(std::ostream& os, const Time& rhs) {
  os << rhs.ToString();
  return os;
}

}  // namespace unify_api
