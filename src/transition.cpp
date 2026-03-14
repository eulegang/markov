#include <cassert>
#include <cstdlib>
#include <cstring>
#include <new>

#include "markov.h"
#include "priv.h"

void markov::transition::invariant() const {
  assert(base);
  assert(len);

  for (markov::state from = 0; from < len; from++) {

    double total = 1.0;
    for (markov::state to = 0; to < len; to++) {
      total -= base[len * from + to];
    }

    assert(std::abs(total) < MARKOV_ERROR_MARGIN);
  }
}

markov::transition::transition(const markov::transition::weights &w) {
  len = w.len;
  if (!(base = (double *)malloc(sizeof(double) * len * len)))
    throw std::bad_alloc();

  size_t blen = subbuffer_len(len);
  for (markov::state state = 0; state < len; state++) {
    markov::row *row = (markov::row *)(w.base + (blen * state));

    double total = row->total;
    uint32_t *ptr = row->offset();

    for (size_t i = 0; i < len; i++) {
      base[len * state + i] = ptr[i] / total;
    }
  }

  check_invariant();
}

markov::transition::~transition() { free(base); }

markov::transition::transition(const markov::transition &t) {
  len = t.len;
  if (!(base = (double *)malloc(sizeof(double) * len * len)))
    throw std::bad_alloc();

  memcpy(base, t.base, sizeof(double) * len * len);
  check_invariant();
}

markov::transition &markov::transition::operator=(const markov::transition &t) {
  len = t.len;
  if (!(base = (double *)malloc(sizeof(double) * len * len)))
    throw std::bad_alloc();

  memcpy(base, t.base, sizeof(double) * len * len);

  check_invariant();

  return *this;
}

double markov::transition::ratio(state from, state to) const {
  assert(from < len);
  assert(to < len);

  return base[len * from + to];
}
