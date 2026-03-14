
#include <gtest/gtest.h>

#include "markov.h"

TEST(chain, apply) {
  markov::distribution::weights dw{2};

  dw.insert(0, 3);
  dw.insert(1, 5);

  markov::distribution d{dw};

  markov::transition::weights tw{2};

  tw.insert(0, 0, 4);
  tw.insert(0, 1, 4);
  tw.insert(1, 0, 1);
  tw.insert(1, 1, 7);

  markov::transition t{tw};

  markov::distribution next = t(d);

  double da = 3.0 / 8.0;
  double db = 5.0 / 8.0;

  double t00 = 4.0 / 8.0;
  double t01 = 4.0 / 8.0;
  double t10 = 1.0 / 8.0;
  double t11 = 7.0 / 8.0;

  EXPECT_DOUBLE_EQ(next.ratio(0), da * t00 + db * t10);
  EXPECT_DOUBLE_EQ(next.ratio(1), da * t01 + db * t11);
}

TEST(chain, in_place) {
  markov::distribution::weights dw{2};

  dw.insert(0, 3);
  dw.insert(1, 5);

  markov::distribution d{dw};

  markov::transition::weights tw{2};

  tw.insert(0, 0, 4);
  tw.insert(0, 1, 4);
  tw.insert(1, 0, 1);
  tw.insert(1, 1, 7);

  markov::transition t{tw};
  markov::distribution::weights nw{2};
  nw.insert(0, 1);
  nw.insert(1, 1);
  markov::distribution next{nw};

  t(next, d);

  double da = 3.0 / 8.0;
  double db = 5.0 / 8.0;

  double t00 = 4.0 / 8.0;
  double t01 = 4.0 / 8.0;
  double t10 = 1.0 / 8.0;
  double t11 = 7.0 / 8.0;

  EXPECT_DOUBLE_EQ(next.ratio(0), da * t00 + db * t10);
  EXPECT_DOUBLE_EQ(next.ratio(1), da * t01 + db * t11);
}
