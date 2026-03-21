#ifndef _MARKOV_NAPI
#define _MARKOV_NAPI

#include <cassert>
#include <node/node_api.h>

#include "napi.h"

#define NAPI_CHECK(expr) assert((expr) == napi_ok)

#define DECLARE_NAPI_METHOD(name, func)                                        \
  {name, 0, func, 0, 0, 0, napi_default, 0}

namespace markov {
namespace node {
extern napi_ref distribution_cons;
extern napi_ref distribution_weight_cons;
extern napi_ref transition_cons;
extern napi_ref transition_weight_cons;

napi_ref define_distribution(napi_env env);
napi_ref define_distribution_weight(napi_env env);
napi_ref define_transition_weight(napi_env env);
napi_ref define_transition(napi_env env);
} // namespace node
} // namespace markov

#endif
