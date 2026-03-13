#ifndef _MARKOV
#define _MARKOV

#include <cstddef>
#include <cstdint>

namespace markov {
using state = uint8_t;

class transitions final {

public:
  class weights final {
    uint32_t *base;
    size_t len;

  public:
    weights(uint8_t states);
    void insert(state from, state to, uint32_t n = 1);
    size_t size() const;
  };
};
} // namespace markov

#endif
