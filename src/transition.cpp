
#include "markov.h"
#include "priv.h"
#include <cstdlib>
#include <cstring>
#include <new>

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
}

markov::transition::~transition() { free(base); }

markov::transition::transition(const markov::transition &t) {
  len = t.len;
  if ((base = (double *)malloc(sizeof(double) * len * len)))
    throw std::bad_alloc();

  memcpy(base, t.base, sizeof(double) * len * len);
}
markov::transition &markov::transition::operator=(const markov::transition &t) {
  len = t.len;
  if ((base = (double *)malloc(sizeof(double) * len * len)))
    throw std::bad_alloc();

  memcpy(base, t.base, sizeof(double) * len * len);

  return *this;
}

double markov::transition::ratio(state from, state to) const {
  return base[len * from + to];
}
