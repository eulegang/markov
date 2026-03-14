#include <cassert>
#include <cstdlib>
#include <cstring>
#include <new>

#include "markov.h"
#include "priv.h"

void markov::transition::weights::invariant() const {
  assert(base);
  assert(len);

  size_t blen = subbuffer_len(len);
  for (markov::state state = 0; state < len; state++) {
    markov::row *row = (markov::row *)(base + (blen * state));
    uint32_t *base = row->offset();
    uint64_t total = 0;

    for (size_t i = 0; i < len; i++) {
      total += base[i];
    }

    assert(total == row->total);
  }
}

markov::transition::weights::weights(uint8_t states) {
  len = states;

  size_t blen = markov::subbuffer_len(states);
  if (!(base = (uint32_t *)malloc(sizeof(uint32_t) * blen * states)))
    throw std::bad_alloc();

  memset(base, 0, sizeof(uint32_t) * blen * states);

  check_invariant();
}

markov::transition::weights::~weights() { free(base); }

markov::transition::weights::weights(const markov::transition::weights &w)
    : len{w.len} {
  size_t blen = markov::subbuffer_len(w.len);
  if (!(base = (uint32_t *)malloc(sizeof(uint32_t) * blen * w.len)))
    throw std::bad_alloc();

  memcpy(base, w.base, sizeof(uint32_t) * blen * w.len);

  check_invariant();
}

markov::transition::weights &
markov::transition::weights::operator=(const markov::transition::weights &w) {
  free(base);
  size_t blen = markov::subbuffer_len(w.len);
  if (!(base = (uint32_t *)malloc(sizeof(uint32_t) * blen * w.len)))
    throw std::bad_alloc();

  memcpy(base, w.base, sizeof(uint32_t) * blen * w.len);
  len = w.len;

  check_invariant();

  return *this;
}

markov::transition::weights &
markov::transition::weights::operator+=(const markov::transition::weights &w) {
  assert(len == w.len);

  size_t blen = subbuffer_len(len);
  for (markov::state from = 0; from < len; from++) {
    markov::row *row = (markov::row *)(base + (blen * from));
    markov::row *orow = (markov::row *)(w.base + (blen * from));

    uint32_t *nums = row->offset(0);
    uint32_t *onums = orow->offset(0);

    row->total += orow->total;
    for (size_t i = 0; i < len; i++) {
      *(nums++) += *(onums++);
    }
  }

  check_invariant();

  return *this;
}

void markov::transition::weights::insert(markov::state from, markov::state to,
                                         uint32_t n) {
  size_t blen = subbuffer_len(len);
  markov::row *row = (markov::row *)(base + (blen * from));

  markov::transition_entry entry = row->entry(to);
  entry.add(n);

  check_invariant();
}

size_t markov::transition::weights::size() const {
  size_t total = 0;
  size_t blen = subbuffer_len(len);
  for (markov::state state = 0; state < len; state++) {
    markov::row *row = (markov::row *)(base + (blen * state));
    total += row->total;
  }

  return total;
}
