#include "markov.h"
#include "priv.h"

#include <cstdlib>
#include <cstring>

markov::transitions::weights::weights(uint8_t states) {
  len = states;

  size_t blen = markov::subbuffer_len(states);
  base = (uint32_t *)malloc(sizeof(uint32_t) * blen * states);
  memset(base, 0, sizeof(uint32_t) * blen * states);
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
