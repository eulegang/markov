#ifndef _MARKOV_PRIV
#define _MARKOV_PRIV

#include <cstddef>
#include <cstdint>

namespace markov {

struct transition_entry {
  uint64_t &total;
  uint32_t &col;

  void add(uint32_t amount) {
    total += amount;
    col += amount;
  }
};

struct row {
  uint64_t total;

  transition_entry entry(size_t off) {
    transition_entry entry{
        .total = this->total,
        .col = *((uint32_t *)this + off + 2),
    };

    return entry;
  }
};

inline size_t subbuffer_len(uint8_t states) {
  size_t len = states;
  len += 2;
  len += (len & 1);
  return len;
}

} // namespace markov

#endif
