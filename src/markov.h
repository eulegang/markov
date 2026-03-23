#ifndef _MARKOV
#define _MARKOV

#include <cstddef>
#include <cstdint>

namespace markov {
using state = uint8_t;

class state_iter final {
  state cur;

public:
  state_iter(state cur) : cur{cur} {}
  state operator*() const { return cur; };
  state_iter operator++() { return state_iter(cur++); };
  bool operator==(const state_iter &other) const { return cur == other.cur; };
};

class states final {
  state max;

public:
  states(state max) : max{max} {}
  markov::state size() const { return max; }
  state_iter begin() { return state_iter(0); }
  state_iter end() { return state_iter(max); }
};

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
    uint32_t operator[](state state) const;
    void insert(state state, uint32_t n = 1);
    void extend(const weights &);
    size_t size() const;

    markov::states states();
  };

  distribution(const weights &);
  distribution(const transition &, const distribution &);
  ~distribution();
  distribution(const distribution &);
  distribution &operator=(const distribution &);

  distribution(distribution &&) = delete;
  distribution &operator=(distribution &&) = delete;

  double operator[](markov::state state) const;

  markov::states states();
  void from(const transition &, const distribution &);

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
    uint32_t operator[](markov::state from, markov::state to) const;

    weights &operator+=(const weights &);
    void insert(state from, state to, uint32_t n = 1);
    void extend(const weights &);
    markov::states states() const;
    size_t size() const;
    bool is_ready() const;
  };

  transition(const weights &);
  ~transition();

  transition(const transition &);
  transition &operator=(const transition &);

  transition(transition &&) = delete;
  transition &operator=(transition &&) = delete;

  double operator[](markov::state from, markov::state to) const;

  distribution operator()(const distribution &) const;
  void operator()(distribution &, const distribution &) const;

  markov::states states() const;

  double ratio(state from, state to) const;
};
} // namespace markov

#endif
