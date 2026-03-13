#include "markov.h"
#include "priv.h"

#include <cassert>
#include <cstdlib>
#include <cstring>
#include <new>

markov::transitions::weights::weights(uint8_t states) {
  len = states;

  size_t blen = markov::subbuffer_len(states);
  if (!(base = (uint32_t *)malloc(sizeof(uint32_t) * blen * states)))
    throw std::bad_alloc();

  memset(base, 0, sizeof(uint32_t) * blen * states);
}

markov::transitions::weights::~weights() { free(base); }

markov::transitions::weights::weights(const markov::transitions::weights &w)
    : len{w.len} {
  size_t blen = markov::subbuffer_len(w.len);
  if (!(base = (uint32_t *)malloc(sizeof(uint32_t) * blen * w.len)))
    throw std::bad_alloc();

  memcpy(base, w.base, sizeof(uint32_t) * blen * w.len);
}

markov::transitions::weights &
markov::transitions::weights::operator=(const markov::transitions::weights &w) {
  free(base);
  size_t blen = markov::subbuffer_len(w.len);
  if (!(base = (uint32_t *)malloc(sizeof(uint32_t) * blen * w.len)))
    throw std::bad_alloc();

  memcpy(base, w.base, sizeof(uint32_t) * blen * w.len);
  len = w.len;
  return *this;
}

markov::transitions::weights &markov::transitions::weights::operator+=(
    const markov::transitions::weights &w) {
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

  return *this;
}

void markov::transitions::weights::insert(markov::state from, markov::state to,
                                          uint32_t n) {
  size_t blen = subbuffer_len(len);
  markov::row *row = (markov::row *)(base + (blen * from));

  markov::transition_entry entry = row->entry(to);
  entry.add(n);
}

size_t markov::transitions::weights::size() const {
  size_t total = 0;
  size_t blen = subbuffer_len(len);
  for (markov::state state = 0; state < len; state++) {
    markov::row *row = (markov::row *)(base + (blen * state));
    total += row->total;
  }

  return total;
}
