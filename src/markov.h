#ifndef _MARKOV
#define _MARKOV

#include <cstddef>
#include <cstdint>

namespace markov {
using state = uint8_t;

class transition;

class distribution final {
  double *base;
  size_t len;

  void invariant() const;

  friend class transition;

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
  distribution(const transition &, const distribution &);
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

  friend class distribution;

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

  distribution operator()(const distribution &) const;

  double ratio(state from, state to) const;
};
} // namespace markov

#endif
