#include <gtest/gtest.h>

#include "markov.h"

TEST(transition_weights, accumulate) {
  markov::transition::weights w{2};
  w.insert(0, 0, 5);
  w.insert(0, 1);
  w.insert(1, 0, 5);
  w.insert(1, 1, 30);

  EXPECT_EQ(w.size(), 41);
}

TEST(transition_weights, sum) {
  markov::transition::weights w{2};
  w.insert(0, 0, 5);
  w.insert(1, 0, 5);

  markov::transition::weights x{2};
  x.insert(1, 1, 3);
  x.insert(0, 1, 3);

  markov::transition::weights y{2};

  y += w;
  y += x;

  EXPECT_EQ(w.size(), 10);
  EXPECT_EQ(x.size(), 6);
  EXPECT_EQ(y.size(), 16);
}
