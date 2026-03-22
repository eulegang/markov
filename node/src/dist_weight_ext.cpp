
#include <cstdint>
#include <limits>
#include <node/js_native_api.h>
#include <node/js_native_api_types.h>
#include <string>

#include "extension.h"
#include "markov.h"

namespace markov {
namespace node {
napi_ref distribution_weight_cons;

void dist_weight_dtor(napi_env, void *data, void *) {
  auto weight = static_cast<markov::distribution::weights *>(data);
  delete weight;
}

napi_value dist_weight_ctor(napi_env env, napi_callback_info info) {
  napi node{env};

  napi_value target = node.new_target(info);

  if (target != nullptr) {
    size_t argc = 1;
    napi_value jsthis;
    napi_value argv[1];
    node.cb_info(info, &argc, argv, &jsthis);

    if (argc != 1) {
      node.throw_error("invalid arguments");
      return NULL;
    }

    if (node.typeof(argv[0]) == napi_number) {
      uint32_t len = node.get_uint32(argv[0]);

      if (len > std::numeric_limits<markov::state>::max()) {
        node.throw_error("invalid number of states");
        return NULL;
      }

      markov::distribution::weights *weights =
          new markov::distribution::weights{(markov::state)len};

      node.wrap(jsthis, weights, dist_weight_dtor);

      return jsthis;
    } else if (node.instanceof(argv[0], distribution_weight_cons)) {
      markov::distribution::weights *weights =
          node.unwrap<markov::distribution::weights>(argv[0]);

      markov::distribution::weights *copy =
          new markov::distribution::weights{*weights};

      node.wrap(jsthis, copy, dist_weight_dtor);

      return jsthis;
    } else {
      if (!node.is_array(argv[0])) {
        node.throw_error("need array of weights");
        return NULL;
      }

      uint32_t length = node.array_length(argv[0]);
      if (length > std::numeric_limits<markov::state>::max()) {
        node.throw_error("too many states");
        return NULL;
      }

      if (length == 0) {
        node.throw_error("too little states");
        return NULL;
      }

      markov::distribution::weights *weights =
          new markov::distribution::weights{(uint8_t)length};

      for (uint32_t i = 0; i < length; i++) {
        napi_value elem = node.array_elem(argv[0], i);

        if (node.typeof(elem) != napi_number) {
          delete weights;
          node.throw_error("invalid weight");
          return NULL;
        }

        weights->insert(i, node.get_uint32(elem));
      }

      node.wrap(jsthis, weights, dist_weight_dtor);

      return jsthis;
    }
  } else {
    node.throw_error("use new keyword");
    return NULL;
  }
}

napi_value dist_weight_get(napi_env env, napi_callback_info info) {
  napi node{env};

  napi_value jsthis;
  napi_value argv[1];
  size_t argc = 1;

  node.cb_info(info, &argc, argv, &jsthis);

  if (argc != 1 && node.typeof(argv[0]) != napi_number) {
    node.throw_error("invalid arguments");
    return NULL;
  }

  markov::distribution::weights *weights =
      node.unwrap<markov::distribution::weights>(jsthis);

  if (uint32_t state = node.get_uint32(argv[0]);
      state < weights->states().size()) {
    uint32_t w = (*weights)[state];

    return node.create_uint32(w);
  } else {
    node.throw_error("invalid state");

    return NULL;
  }
}

napi_value dist_weight_total(napi_env env, napi_callback_info info) {
  napi node{env};

  napi_value jsthis;

  node.cb_info(info, NULL, NULL, &jsthis);

  markov::distribution::weights *weights =
      node.unwrap<markov::distribution::weights>(jsthis);

  return node.create_uint32(weights->size());
}

napi_value dist_weight_insert(napi_env env, napi_callback_info info) {
  napi node{env};

  napi_value jsthis;
  napi_value argv[2];
  size_t argc = 2;
  node.cb_info(info, &argc, argv, &jsthis);

  if (argc < 1) {
    node.throw_error("invalid arguments");
    return NULL;
  }

  markov::distribution::weights *weights;
  node.unwrap(jsthis, reinterpret_cast<void **>(&weights));

  uint32_t state = 255;
  uint32_t n = 1;

  if (node.typeof(argv[0]) != napi_number) {
    node.throw_error("expected integer for state");
    return NULL;
  }

  state = node.get_uint32(argv[0]);
  if (state >= weights->states().size()) {
    node.throw_error("out of bound state provided");
    return NULL;
  }

  if (argc > 1) {
    if (node.typeof(argv[1]) != napi_number) {
      node.throw_error("expected integer for weight");
      return NULL;
    }

    n = node.get_uint32(argv[1]);
  }

  weights->insert(state, n);

  return NULL;
}

napi_value dist_weight_toString(napi_env env, napi_callback_info info) {
  napi node{env};

  napi_value jsthis;
  node.cb_info(info, NULL, NULL, &jsthis);

  markov::distribution::weights *weights;
  node.unwrap(jsthis, reinterpret_cast<void **>(&weights));

  std::string res{};
  for (const auto &state : weights->states()) {
    uint32_t count = (*weights)[state];

    res += std::to_string(count);
    res += " ";
  }

  if (res.length()) {
    res.pop_back();
  }

  return node.create_utf8(res);
}

napi_value dist_weight_states(napi_env env, napi_callback_info info) {
  napi node{env};

  napi_value jsthis;

  node.cb_info(info, NULL, NULL, &jsthis);

  markov::distribution::weights *weights =
      node.unwrap<markov::distribution::weights>(jsthis);

  return node.create_uint32(weights->states().size());
}

napi_value dist_weight_extend(napi_env env, napi_callback_info info) {
  napi node{env};

  size_t argc = 1;
  napi_value jsthis;
  napi_value argv[1];

  node.cb_info(info, &argc, argv, &jsthis);

  if (argc != 1) {
    node.throw_error("invalid arguments");
    return NULL;
  }

  markov::distribution::weights *subject =
      node.unwrap<markov::distribution::weights>(jsthis);

  markov::distribution::weights *other =
      node.unwrap<markov::distribution::weights>(argv[0]);

  if (subject->states().size() != other->states().size()) {
    node.throw_error("incompatible state count");
    return NULL;
  }

  subject->extend(*other);

  return NULL;
}
} // namespace node
} // namespace markov
;
napi_ref markov::node::define_distribution_weight(napi_env env) {
  napi_property_descriptor dist_props[] = {
      DECLARE_NAPI_METHOD("insert", dist_weight_insert),
      DECLARE_NAPI_METHOD("get", dist_weight_get),
      DECLARE_NAPI_METHOD("total", dist_weight_total),
      DECLARE_NAPI_METHOD("toString", dist_weight_toString),
      DECLARE_NAPI_GETTER("states", dist_weight_states),
      DECLARE_NAPI_METHOD("extend", dist_weight_extend),
  };

  napi_value dist_cons;
  NAPI_CHECK(napi_define_class(env, "Weights", NAPI_AUTO_LENGTH,
                               dist_weight_ctor, NULL, 6, dist_props,
                               &dist_cons));

  napi_create_reference(env, dist_cons, 1, &distribution_weight_cons);

  return distribution_weight_cons;
}
