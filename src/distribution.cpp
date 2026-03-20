#include <cassert>
#include <cstdlib>
#include <new>

#include "markov.h"
#include "priv.h"

void markov::distribution::invariant() const {
  assert(base);
  assert(len);

  double total = 1.0;
  for (markov::state state = 0; state < len; state++) {
    total -= base[state];
  }

  assert(std::abs(total) < MARKOV_ERROR_MARGIN);
}

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

  check_invariant();
}

markov::distribution::distribution(const markov::transition &t,
                                   const distribution &d) {
  assert(t.len == d.len);

  len = t.len;
  if (!(base = (double *)malloc(sizeof(double) * len)))
    throw std::bad_alloc();

  from(t, d);
}

markov::distribution::~distribution() { free(base); }

markov::distribution::distribution(const markov::distribution &d) {
  len = d.len;

  if (!(base = (double *)malloc(sizeof(double) * len)))
    throw std::bad_alloc();

  for (size_t i = 0; i < len; i++) {
    base[i] = d.base[i];
  }

  check_invariant();
}

markov::distribution &
markov::distribution::operator=(const markov::distribution &d) {
  len = d.len;

  if (!(base = (double *)malloc(sizeof(double) * len)))
    throw std::bad_alloc();

  for (size_t i = 0; i < len; i++) {
    base[i] = d.base[i];
  }

  check_invariant();

  return *this;
}

double markov::distribution::ratio(markov::state state) const {
  assert(state < len);
  return base[state];
}

double markov::distribution::operator[](markov::state state) const {
  assert(state < len);
  return base[state];
}

void markov::distribution::from(const markov::transition &t,
                                const markov::distribution &d) {
  for (markov::state i = 0; i < len; i++) {
    double cur = 0;
    for (markov::state from = 0; from < len; from++) {
      cur += t.base[from * len + i] * d.base[from];
    }

    base[i] = cur;
  }
}

markov::states markov::distribution::states() { return markov::states(len); }
