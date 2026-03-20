#include "extension.h"
#include "markov.h"
#include <cstdlib>
#include <limits>
#include <memory>
#include <node/js_native_api_types.h>

namespace markov {
namespace node {

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

  size_t argc = 2;
  napi_value argv[3];
  napi_value jsthis;

  node.cb_info(info, &argc, argv, &jsthis);

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

napi_value trans_weight_toString(napi_env env, napi_callback_info info) {
  napi node{env};
  napi_value jsthis;
  node.cb_info(info, NULL, NULL, &jsthis);

  markov::transition::weights *weights;
  node.unwrap(jsthis, reinterpret_cast<void **>(&weights));

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

} // namespace node
} // namespace markov

napi_ref markov::node::define_transition_weight(napi_env env) {
  markov::node::napi node{env};

  napi_property_descriptor trans_props[] = {
      DECLARE_NAPI_METHOD("insert", trans_weight_insert),
      DECLARE_NAPI_METHOD("toString", trans_weight_toString),
  };

  napi_value transw_cons;
  NAPI_CHECK(napi_define_class(env, "Weights", NAPI_AUTO_LENGTH,
                               trans_weight_ctor, NULL, 2, trans_props,
                               &transw_cons));

  return node.ref(transw_cons);
}
