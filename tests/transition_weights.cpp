#include <gtest/gtest.h>

#include "markov.h"

TEST(transitions_weights, accumulate) {
  markov::transitions::weights w{2};
  w.insert(0, 0, 5);
  w.insert(0, 1);
  w.insert(1, 0, 5);
  w.insert(1, 1, 30);

  EXPECT_EQ(w.size(), 41);
}
