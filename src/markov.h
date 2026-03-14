#ifndef _MARKOV
#define _MARKOV

#include <cstddef>
#include <cstdint>

namespace markov {
using state = uint8_t;

class distribution final {
  double *base;
  size_t len;

  void invariant() const;

public:
  class weights final {
    uint32_t *base;
    size_t len;

    friend class distribution;

    void invariant() const;

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

  distribution(const weights &);
  ~distribution();
  distribution(const distribution &);
  distribution &operator=(const distribution &);

  distribution(distribution &&) = delete;
  distribution &operator=(distribution &&) = delete;

  double ratio(state state) const;
};

class transition final {
  double *base;
  size_t len;

  void invariant() const;

public:
  class weights final {
    uint32_t *base;
    size_t len;

    friend class transition;

    void invariant() const;

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
