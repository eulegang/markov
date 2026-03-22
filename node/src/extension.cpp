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
  napi_ref transw;
  napi_ref trans;
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
  markov::node::napi node{env};

  napi_value version;
  napi_create_int64(env, 0, &version);

  markov::node::Refs *refs = new markov::node::Refs;

  napi_ref distw_ref = markov::node::define_distribution_weight(env);
  napi_ref dist_ref = markov::node::define_distribution(env);
  napi_ref transw_ref = markov::node::define_transition_weight(env);
  napi_ref trans_ref = markov::node::define_transition(env);

  refs->dist = dist_ref;
  refs->distw = distw_ref;
  refs->transw = transw_ref;
  refs->trans = trans_ref;

  napi_set_instance_data(env, refs, cleanup, nullptr);

  node.set_prop(exports, "version", version);
  node.set_prop(exports, "Distribution", node.deref(dist_ref));
  node.set_prop(exports, "Transition", node.deref(trans_ref));

  node.set_prop(node.deref(dist_ref), "Weights", node.deref(distw_ref));
  node.set_prop(node.deref(trans_ref), "Weights", node.deref(transw_ref));

  return exports;
}

NAPI_MODULE("markov", init);
