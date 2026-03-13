#include "markov.h"
#include <cstdlib>
#include <new>

markov::distribution::weights::weights(uint8_t states) {

  len = states;
  if (!(base = (uint32_t *)malloc(sizeof(uint32_t) * states)))
    throw std::bad_alloc();

  for (size_t i = 0; i < len; i++) {
    base[i] = 0.0;
  }
}

markov::distribution::weights::weights(const markov::distribution::weights &w) {
  len = w.len;
  if (!(base = (uint32_t *)malloc(sizeof(uint32_t) * len)))
    throw std::bad_alloc();

  for (size_t i = 0; i < len; i++) {
    base[i] = w.base[i];
  }
}

markov::distribution::weights &markov::distribution::weights::operator=(
    const markov::distribution::weights &w) {
  free(base);

  len = w.len;
  if (!(base = (uint32_t *)malloc(sizeof(uint32_t) * len)))
    throw std::bad_alloc();

  for (size_t i = 0; i < len; i++) {
    base[i] = w.base[i];
  }

  return *this;
}

markov::distribution::weights::~weights() { free(base); }

size_t markov::distribution::weights::size() const {
  size_t total = 0;

  for (size_t i = 0; i < len; i++) {
    total += base[i];
  }

  return total;
}

void markov::distribution::weights::insert(markov::state state, uint32_t n) {
  base[state] += n;
}
