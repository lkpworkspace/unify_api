#include <gtest/gtest.h>

#include "duration.h"
#include "rate.h"
#include "time.h"

TEST(time, test) {
    auto now = common::Time::Now();
    std::cout << now << std::endl;
}
