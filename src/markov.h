#ifndef _MARKOV
#define _MARKOV

#include <cstddef>
#include <cstdint>

namespace markov {
using state = uint8_t;

class distribution final {

public:
  class weights final {
    uint32_t *base;
    size_t len;

  public:
    weights(uint8_t states);
    ~weights();
    weights(const weights &);
    weights(weights &&) = delete;
    weights &operator=(const weights &);
    weights &operator=(weights &&) = delete;

    weights &operator+=(const weights &);
    void insert(state state, uint32_t n = 1);
    size_t size() const;
  };
};

class transition final {
  double *base;
  size_t len;

public:
  class weights final {
    uint32_t *base;
    size_t len;

    friend class transition;

  public:
    weights(uint8_t states);
    ~weights();
    weights(const weights &);
    weights(weights &&) = delete;
    weights &operator=(const weights &);
    weights &operator=(weights &&) = delete;

    weights &operator+=(const weights &);
    void insert(state from, state to, uint32_t n = 1);
    size_t size() const;
  };

  transition(const weights &);
  ~transition();

  transition(const transition &);
  transition &operator=(const transition &);

  transition(transition &&) = delete;
  transition &operator=(transition &&) = delete;

  double ratio(state from, state to) const;
};
} // namespace markov

#endif
