#include "markov.h"
#include <cassert>
#include <cstdlib>
#include <new>

markov::distribution::distribution(const markov::distribution::weights &w) {
  len = w.len;

  if (!(base = (double *)malloc(sizeof(double) * len)))
    throw std::bad_alloc();

  uint32_t total = 0;
  for (size_t i = 0; i < len; i++) {
    total += w.base[i];
  }

  double t = total;
  for (size_t i = 0; i < len; i++) {
    base[i] = w.base[i] / t;
  }
}

markov::distribution::~distribution() { free(base); }

markov::distribution::distribution(const markov::distribution &d) {
  len = d.len;

  if (!(base = (double *)malloc(sizeof(double) * len)))
    throw std::bad_alloc();

  for (size_t i = 0; i < len; i++) {
    base[i] = d.base[i];
  }
}

markov::distribution &
markov::distribution::operator=(const markov::distribution &d) {
  len = d.len;

  if (!(base = (double *)malloc(sizeof(double) * len)))
    throw std::bad_alloc();

  for (size_t i = 0; i < len; i++) {
    base[i] = d.base[i];
  }

  return *this;
}

double markov::distribution::ratio(markov::state state) const {
  assert(state < len);
  return base[state];
}
