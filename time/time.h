/****************************************************************************
Copyright (c) 2023, likepeng
All rights reserved.

Author: likepeng <likepeng0418@163.com>
****************************************************************************/
#pragma once
#include <string>

#include "time/duration.h"

namespace unify_api {

class Time {
 public:
  static const Time MAX;
  static const Time MIN;
  Time() = default;
  explicit Time(int64_t nanoseconds);
  explicit Time(int nanoseconds);
  explicit Time(double seconds);
  Time(uint32_t seconds, uint32_t nanoseconds);
  Time(const Time& other);
  Time& operator=(const Time& other);

  static Time Now();
  static Time MonoTime();

  static void SleepUntil(const Time& time);

  double ToSecond() const;

  int64_t ToNanosecond() const;

  std::string ToString() const;

  bool IsZero() const;

  Duration operator-(const Time& rhs) const;
  Time operator+(const Duration& rhs) const;
  Time operator-(const Duration& rhs) const;
  Time& operator+=(const Duration& rhs);
  Time& operator-=(const Duration& rhs);
  bool operator==(const Time& rhs) const;
  bool operator!=(const Time& rhs) const;
  bool operator>(const Time& rhs) const;
  bool operator<(const Time& rhs) const;
  bool operator>=(const Time& rhs) const;
  bool operator<=(const Time& rhs) const;

 private:
  int64_t nanoseconds_ = 0;
};

std::ostream& operator<<(std::ostream& os, const Time& rhs);

}  // namespace unify_api
