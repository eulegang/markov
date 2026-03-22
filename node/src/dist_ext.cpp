#include "extension.h"
#include "markov.h"
#include <format>
#include <node/js_native_api.h>

namespace markov {
namespace node {
napi_ref distribution_cons;

void dist_dtor(napi_env, void *data, void *) {
  auto weight = static_cast<markov::distribution *>(data);
  delete weight;
}

napi_value dist_ctor(napi_env env, napi_callback_info info) {
  markov::node::napi node{env};
  napi_value target = node.new_target(info);

  if (target != nullptr) {
    size_t argc = 2;
    napi_value jsthis;
    napi_value argv[2];

    node.cb_info(info, &argc, argv, &jsthis);

    markov::distribution *dist;

    if (argc == 2 && node.instanceof(argv[0], transition_cons) &&
        node.instanceof(argv[1], distribution_cons)) {

      markov::transition *transition = node.unwrap<markov::transition>(argv[0]);
      markov::distribution *distribution =
          node.unwrap<markov::distribution>(argv[1]);

      dist = new markov::distribution{*transition, *distribution};

    } else if (argc == 1 &&
               node.instanceof(argv[0], node.deref(distribution_weight_cons))) {
      markov::distribution::weights *weights =
          node.unwrap<markov::distribution::weights>(argv[0]);

      if (weights->size() == 0) {
        node.throw_error("invalid weights");
        return NULL;
      }

      // node.unwrap(argv[0], reinterpret_cast<void **>(&weights));

      dist = new markov::distribution{*weights};
    } else {
      node.throw_error("distrbutions must be constructed with weights");
      return NULL;
    }

    node.wrap(jsthis, dist, dist_dtor);

    return jsthis;
  } else {
    node.throw_error("use new keyword");
    return NULL;
  }
}

napi_value dist_get(napi_env env, napi_callback_info info) {
  markov::node::napi node{env};

  size_t argc = 1;
  napi_value argv[1];
  napi_value jsthis;
  node.cb_info(info, &argc, argv, &jsthis);

  if (argc < 1) {
    node.throw_error("invalid arguments");
    return NULL;
  }

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

  markov::distribution *dist = node.unwrap<markov::distribution>(jsthis);

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

napi_ref markov::node::define_distribution(napi_env env) {
  markov::node::napi node{env};

  napi_property_descriptor dist_props[] = {
      DECLARE_NAPI_METHOD("get", dist_get),
      DECLARE_NAPI_METHOD("toString", dist_toString),
  };

  napi_value dist_cons;
  NAPI_CHECK(napi_define_class(env, "Distribution", NAPI_AUTO_LENGTH, dist_ctor,
                               NULL, 2, dist_props, &dist_cons));

  distribution_cons = node.ref(dist_cons);

  return distribution_cons;
}
