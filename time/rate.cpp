/****************************************************************************
Copyright (c) 2023, likepeng
All rights reserved.

Author: likepeng <likepeng0418@163.com>
****************************************************************************/
#include "time/rate.h"

namespace unify_api {

Rate::Rate(double frequency)
    : start_(Time::Now()),
      expected_cycle_time_(1.0 / frequency),
      actual_cycle_time_(0.0) {}

Rate::Rate(uint64_t nanoseconds)
    : start_(Time::Now()),
      expected_cycle_time_(static_cast<int64_t>(nanoseconds)),
      actual_cycle_time_(0.0) {}

Rate::Rate(const Duration& d)
    : start_(Time::Now()), expected_cycle_time_(d), actual_cycle_time_(0.0) {}

void Rate::Sleep() {
  Time expected_end = start_ + expected_cycle_time_;

  Time actual_end = Time::Now();

  // detect backward jumps in time
  if (actual_end < start_) {
    expected_end = actual_end + expected_cycle_time_;
  }

  // calculate the time we'll sleep for
  Duration sleep_time = expected_end - actual_end;

  // set the actual amount of time the loop took in case the user wants to kNow
  actual_cycle_time_ = actual_end - start_;

  // make sure to reset our start time
  start_ = expected_end;

  // if we've taken too much time we won't sleep
  if (sleep_time < Duration(0.0)) {
    // if we've jumped forward in time, or the loop has taken more than a full
    // extra
    // cycle, reset our cycle
    if (actual_end > expected_end + expected_cycle_time_) {
        start_ = actual_end;
    }
    // return false to show that the desired rate was not met
    return;
  }

  Time::SleepUntil(expected_end);
}

void Rate::Reset() { start_ = Time::Now(); }

Duration Rate::CycleTime() const { return actual_cycle_time_; }

}  // namespace unify_api
