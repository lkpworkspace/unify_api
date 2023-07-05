/****************************************************************************
Copyright (c) 2023, likepeng
All rights reserved.

Author: likepeng <likepeng0418@163.com>
****************************************************************************/
#pragma once
#include "unify_api_export.h"
#include "time/duration.h"
#include "time/time.h"

namespace unify_api {

class UNIFY_API_EXPORT Rate {
 public:
  explicit Rate(double frequency);
  explicit Rate(uint64_t nanoseconds);
  explicit Rate(const Duration&);
  void Sleep();
  void Reset();
  Duration CycleTime() const;
  Duration ExpectedCycleTime() const { return expected_cycle_time_; }

 private:
  Time start_;
  Duration expected_cycle_time_;
  Duration actual_cycle_time_;
};

}  // namespace unify_api
