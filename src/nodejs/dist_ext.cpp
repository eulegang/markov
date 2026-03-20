#include "extension.h"
#include "markov.h"
#include <format>
#include <node/js_native_api.h>

namespace markov {
namespace node {
static napi_ref weight_cons;

void dist_dtor(napi_env, void *data, void *) {
  auto weight = static_cast<markov::distribution *>(data);
  delete weight;
}

napi_value dist_ctor(napi_env env, napi_callback_info info) {
  markov::node::napi node{env};
  napi_value target = node.new_target(info);

  if (target != nullptr) {
    size_t argc = 1;
    napi_value jsthis;
    napi_value argv[1];

    node.cb_info(info, &argc, argv, &jsthis);
    napi_value wcons = node.deref(weight_cons);

    if (!node.instanceof(argv[0], wcons)) {
      node.throw_error("distrbutions must be constructed with weights");
      return NULL;
    }

    markov::distribution::weights *weights;
    node.unwrap(argv[0], reinterpret_cast<void **>(&weights));

    markov::distribution *dist = new markov::distribution{*weights};

    node.wrap(jsthis, dist, dist_dtor);

    return jsthis;
  } else {
    napi_throw_error(env, nullptr, "use new keyword");
    return NULL;
  }
}

napi_value dist_ratio(napi_env env, napi_callback_info info) {
  markov::node::napi node{env};

  size_t argc = 1;
  napi_value argv[1];
  napi_value jsthis;
  node.cb_info(info, &argc, argv, &jsthis);

  if (node.typeof(argv[0]) != napi_number) {
    node.throw_error("expected state");
    return NULL;
  }

  markov::distribution *dist;

  node.unwrap(jsthis, reinterpret_cast<void **>(&dist));

  uint32_t state = node.get_uint32(argv[0]);
  if (state > dist->states().size()) {
    node.throw_error("invalid state provided");
    return NULL;
  }

  double d = dist->ratio(state);

  return node.create_f64(d);
}

napi_value dist_toString(napi_env env, napi_callback_info info) {
  napi node{env};
  napi_value jsthis;
  node.cb_info(info, NULL, NULL, &jsthis);

  markov::distribution *dist;
  node.unwrap(jsthis, reinterpret_cast<void **>(&dist));

  std::string res{};

  for (const auto &state : dist->states()) {
    double perc = (*dist)[state];
    res += std::format("{:.2f}%", perc * 100.0);
    res += " ";
  }
  res.pop_back();

  return node.create_utf8(res);
}
} // namespace node
} // namespace markov

napi_ref markov::node::define_distribution(napi_env env,
                                           napi_ref weight_cons_ref) {

  markov::node::weight_cons = weight_cons_ref;
  markov::node::napi node{env};

  napi_property_descriptor dist_props[] = {
      DECLARE_NAPI_METHOD("ratio", dist_ratio),
      DECLARE_NAPI_METHOD("toString", dist_toString),
  };

  napi_value dist_cons;
  NAPI_CHECK(napi_define_class(env, "Distribution", NAPI_AUTO_LENGTH, dist_ctor,
                               NULL, 2, dist_props, &dist_cons));

  napi_ref dist_ref;
  napi_create_reference(env, dist_cons, 1, &dist_ref);

  return dist_ref;
}
