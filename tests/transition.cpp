#include <gtest/gtest.h>

#include "markov.h"

TEST(transition, from_weights) {
  markov::transition::weights w{2};
  w.insert(0, 0, 5);
  w.insert(0, 1);
  w.insert(1, 0, 5);
  w.insert(1, 1, 30);

  markov::transition t{w};

  EXPECT_DOUBLE_EQ(t.ratio(0, 0), 5.0 / 6.0);
  EXPECT_DOUBLE_EQ(t.ratio(0, 1), 1.0 / 6.0);
  EXPECT_DOUBLE_EQ(t.ratio(1, 0), 5.0 / 35.0);
  EXPECT_DOUBLE_EQ(t.ratio(1, 1), 30.0 / 35.0);
}
