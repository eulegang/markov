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
napi_ref define_distribution(napi_env env, napi_ref weight_cons_ref);
napi_ref define_distribution_weight(napi_env env);
napi_ref define_transition_weight(napi_env env);
napi_ref define_transition(napi_env env, napi_ref weight_cons_ref);
} // namespace node
} // namespace markov

#endif
