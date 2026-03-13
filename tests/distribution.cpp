#include <gtest/gtest.h>

#include "markov.h"

TEST(distribution, build) {
  markov::distribution::weights w{2};
  w.insert(0, 5);
  w.insert(1, 30);

  markov::distribution d{w};

  EXPECT_DOUBLE_EQ(d.ratio(0), 5.0 / 35.0);
  EXPECT_DOUBLE_EQ(d.ratio(1), 30.0 / 35.0);
}
