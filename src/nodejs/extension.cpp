#include <cassert>
#define NAPI_VERSION 9

#include <node/js_native_api.h>
#include <node/js_native_api_types.h>
#include <node/node_api.h>

#include "extension.h"

namespace markov {
namespace node {
struct Refs {
  napi_ref dist;
  napi_ref distw;
};
} // namespace node
} // namespace markov

void cleanup(napi_env env, void *data, void *hint) {
  (void)hint;
  auto refs = static_cast<markov::node::Refs *>(data);
  napi_status status = napi_delete_reference(env, refs->dist);
  assert(status == napi_ok);
  delete refs;
}

napi_value init(napi_env env, napi_value exports) {
  napi_value version;
  napi_create_int64(env, 0, &version);

  markov::node::Refs *refs = new markov::node::Refs;

  auto [dist_ref, dist_cons] = markov::node::define_distribution(env);
  auto [distw_ref, distw_cons] = markov::node::define_distribution_weight(env);

  refs->dist = dist_ref;
  refs->distw = distw_ref;

  napi_set_instance_data(env, refs, cleanup, nullptr);

  NAPI_CHECK(napi_set_named_property(env, exports, "version", version));
  NAPI_CHECK(napi_set_named_property(env, exports, "Distribution", dist_cons));
  NAPI_CHECK(napi_set_named_property(env, dist_cons, "Weights", distw_cons));

  return exports;
}

NAPI_MODULE("markov", init);
