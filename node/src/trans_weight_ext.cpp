#include "extension.h"
#include "markov.h"
#include <cstdlib>
#include <limits>
#include <node/js_native_api_types.h>

namespace markov {
namespace node {
napi_ref transition_weight_cons;

void trans_weight_dtor(napi_env, void *data, void *) {
  auto weight = static_cast<markov::transition::weights *>(data);
  delete weight;
}

napi_value trans_weight_ctor(napi_env env, napi_callback_info info) {
  napi node{env};

  napi_value target = node.new_target(info);

  if (target != nullptr) {
    size_t argc = 1;
    napi_value jsthis;
    napi_value argv[1];
    node.cb_info(info, &argc, argv, &jsthis);

    if (argc < 1) {
      node.throw_error("invalid arguments");
      return NULL;
    }

    markov::transition::weights *weights;
    if (node.typeof(argv[0]) == napi_number) {
      uint32_t dim = node.get_uint32(argv[0]);

      if (std::numeric_limits<markov::state>::max() < dim) {
        node.throw_error("invalid amount of states");
        return NULL;
      }

      if (dim == 0) {
        node.throw_error("need non empty state space");
        return NULL;
      }

      weights = new markov::transition::weights{(markov::state)dim};
    } else if (node.instanceof(argv[0], transition_weight_cons)) {
      markov::transition::weights *other =
          node.unwrap<markov::transition::weights>(argv[0]);

      weights = new markov::transition::weights{*other};
    } else if (node.is_array(argv[0])) {
      uint32_t dim = node.array_length(argv[0]);

      if (dim == 0) {
        node.throw_error("need non empty weight matrix");
        return NULL;
      }

      for (size_t i = 0; i < dim; i++) {
        napi_value elem = node.array_elem(argv[0], i);

        if (!node.is_array(elem)) {
          node.throw_error("invalid matrix row");
          return NULL;
        }

        if (node.array_length(elem) != dim) {
          node.throw_error("invalid matrix row");
          return NULL;
        }

        for (size_t j = 0; j < dim; j++) {
          napi_value subelem = node.array_elem(elem, j);

          if (node.typeof(subelem) != napi_number) {
            node.throw_error("invalid matrix row");
            return NULL;
          }
        }
      }

      weights = new markov::transition::weights{(markov::state)dim};

      for (size_t i = 0; i < dim; i++) {
        napi_value row = node.array_elem(argv[0], i);

        for (size_t j = 0; j < dim; j++) {
          napi_value cell = node.array_elem(row, j);
          uint32_t n = node.get_uint32(cell);

          weights->insert(i, j, n);
        }
      }
    } else {
      node.throw_error("need array of weights");
      return NULL;
    }

    node.wrap(jsthis, weights, trans_weight_dtor);

    return jsthis;
  } else {
    node.throw_error("use new keyword");
    return NULL;
  }
}

napi_value trans_weight_insert(napi_env env, napi_callback_info info) {
  markov::node::napi node{env};

  size_t argc = 3;
  napi_value argv[3];
  napi_value jsthis;

  node.cb_info(info, &argc, argv, &jsthis);

  if (argc < 2) {
    node.throw_error("invalid arguments");
    return NULL;
  }

  if (node.typeof(argv[0]) != napi_number ||
      node.typeof(argv[1]) != napi_number) {
    node.throw_error("invalid argument");
    return NULL;
  }

  markov::transition::weights *weights;
  node.unwrap(jsthis, reinterpret_cast<void **>(&weights));

  uint32_t from = node.get_uint32(argv[0]);
  uint32_t to = node.get_uint32(argv[1]);

  if (from >= weights->states().size() || to >= weights->states().size()) {
    node.throw_error("invalid argument");
    return NULL;
  }

  uint32_t n = 1;
  if (argc > 2) {
    if (node.typeof(argv[2]) != napi_number) {
      node.throw_error("invalid argument");
      return NULL;
    }

    n = node.get_uint32(argv[2]);
  }

  weights->insert(from, to, n);

  return NULL;
}

napi_value trans_weight_get(napi_env env, napi_callback_info info) {
  napi node{env};

  size_t argc = 2;
  napi_value jsthis;
  napi_value argv[2];

  node.cb_info(info, &argc, argv, &jsthis);

  if (argc != 2 || node.typeof(argv[0]) != napi_number ||
      node.typeof(argv[1]) != napi_number) {
    node.throw_error("invalid arguments");
    return NULL;
  }

  uint32_t row = node.get_uint32(argv[0]);
  uint32_t col = node.get_uint32(argv[1]);
  markov::transition::weights *weights =
      node.unwrap<markov::transition::weights>(jsthis);

  if (row >= weights->states().size() || col >= weights->states().size()) {
    node.throw_error("invalid arguments");
    return NULL;
  }

  uint32_t w = (*weights)[row, col];

  return node.create_uint32(w);
}

napi_value trans_weight_total(napi_env env, napi_callback_info info) {
  napi node{env};

  napi_value jsthis;

  node.cb_info(info, NULL, NULL, &jsthis);

  markov::transition::weights *weights =
      node.unwrap<markov::transition::weights>(jsthis);

  return node.create_uint32(weights->size());
}

napi_value trans_weight_toString(napi_env env, napi_callback_info info) {
  napi node{env};
  napi_value jsthis;
  node.cb_info(info, NULL, NULL, &jsthis);

  markov::transition::weights *weights =
      node.unwrap<markov::transition::weights>(jsthis);

  std::string res{};

  for (const auto &from : weights->states()) {
    for (const auto &to : weights->states()) {
      uint32_t count = (*weights)[from, to];
      res += std::to_string(count);
      res += " ";
    }

    res.pop_back();
    res += "; ";
  }
  res.pop_back();
  res.pop_back();

  return node.create_utf8(res);
}

napi_value trans_weight_extend(napi_env env, napi_callback_info info) {
  napi node{env};

  size_t argc = 1;
  napi_value jsthis;
  napi_value argv[1];

  node.cb_info(info, &argc, argv, &jsthis);

  napi_valuetype ty = node.typeof(argv[0]);
  (void)ty;

  if (argc != 1 || !node.instanceof(argv[0], transition_weight_cons)) {
    node.throw_error("invalid arguments");
    abort();
    return NULL;
  }

  markov::transition::weights *subject =
      node.unwrap<markov::transition::weights>(jsthis);
  markov::transition::weights *other =
      node.unwrap<markov::transition::weights>(argv[0]);

  if (subject->states().size() != other->states().size()) {
    node.throw_error("incompatible state count");
    return NULL;
  }

  subject->extend(*other);

  return NULL;
}

napi_value trans_weight_states(napi_env env, napi_callback_info info) {
  napi node{env};

  napi_value jsthis;

  node.cb_info(info, NULL, NULL, &jsthis);

  markov::transition::weights *weights =
      node.unwrap<markov::transition::weights>(jsthis);

  return node.create_uint32(weights->states().size());
}

} // namespace node
} // namespace markov

napi_ref markov::node::define_transition_weight(napi_env env) {
  markov::node::napi node{env};

  napi_property_descriptor trans_props[] = {
      DECLARE_NAPI_METHOD("insert", trans_weight_insert),
      DECLARE_NAPI_METHOD("extend", trans_weight_extend),
      DECLARE_NAPI_METHOD("toString", trans_weight_toString),
      DECLARE_NAPI_METHOD("get", trans_weight_get),
      DECLARE_NAPI_METHOD("total", trans_weight_total),
      DECLARE_NAPI_GETTER("states", trans_weight_states),
  };

  napi_value transw_cons;
  NAPI_CHECK(napi_define_class(env, "Weights", NAPI_AUTO_LENGTH,
                               trans_weight_ctor, NULL, 6, trans_props,
                               &transw_cons));

  transition_weight_cons = node.ref(transw_cons);
  return transition_weight_cons;
}
