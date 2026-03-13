#include <gtest/gtest.h>

#include "markov.h"

TEST(distribution_weights, accumulate) {
  markov::distribution::weights w{2};
  w.insert(0, 5);
  w.insert(1, 30);

  EXPECT_EQ(w.size(), 35);
}
