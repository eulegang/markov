#ifndef _MARKOV_NAPI
#define _MARKOV_NAPI

#include <cassert>
#include <node/node_api.h>

#define NAPI_CHECK(expr) assert((expr) == napi_ok)

#define DECLARE_NAPI_METHOD(name, func)                                        \
  {name, 0, func, 0, 0, 0, napi_default, 0}

namespace markov {
namespace node {

struct nref {
  napi_ref ref;
  napi_value val;
};

nref define_distribution(napi_env env);
nref define_distribution_weight(napi_env env);
} // namespace node
} // namespace markov

#endif
