
#include <limits>
#include <node/js_native_api.h>
#include <node/js_native_api_types.h>
#include <string>

#include "extension.h"
#include "markov.h"

namespace markov {
namespace node {
void dist_weight_dtor(napi_env, void *data, void *) {
  auto weight = static_cast<markov::distribution::weights *>(data);
  delete weight;
}

napi_value dist_weight_ctor(napi_env env, napi_callback_info info) {
  napi_value target;

  NAPI_CHECK(napi_get_new_target(env, info, &target));

  if (target != nullptr) {
    size_t argc = 1;
    napi_value jsthis;
    napi_value argv[1];
    NAPI_CHECK(napi_get_cb_info(env, info, &argc, argv, &jsthis, nullptr));

    bool is_array;
    NAPI_CHECK(napi_is_array(env, argv[0], &is_array));
    if (!is_array) {
      NAPI_CHECK(napi_throw_error(env, nullptr, "need array of weights"));
      return NULL;
    }

    uint32_t length;
    NAPI_CHECK(napi_get_array_length(env, argv[0], &length));
    if (length > std::numeric_limits<uint8_t>::max()) {
      NAPI_CHECK(napi_throw_error(env, NULL, "too many states"));
    }
    markov::distribution::weights *weights =
        new markov::distribution::weights{(uint8_t)length};

    for (uint32_t i = 0; i < length; i++) {
      napi_value elem;
      NAPI_CHECK(napi_get_element(env, argv[0], i, &elem));

      napi_valuetype ty;
      NAPI_CHECK(napi_typeof(env, elem, &ty));

      if (ty != napi_number) {
        delete weights;
        NAPI_CHECK(napi_throw_error(env, NULL, "invalid weight"));
        return NULL;
      }

      uint32_t weight;
      NAPI_CHECK(napi_get_value_uint32(env, elem, &weight));

      weights->insert(i, weight);
    }

    NAPI_CHECK(napi_wrap(env, jsthis, weights, dist_weight_dtor, NULL, NULL));

    return jsthis;
  } else {
    napi_throw_error(env, nullptr, "use new keyword");
    return NULL;
  }
}

napi_value dist_weight_insert(napi_env env, napi_callback_info info) {
  (void)env;
  (void)info;

  return NULL;
}

napi_value dist_weight_toString(napi_env env, napi_callback_info info) {
  napi_value jsthis;

  NAPI_CHECK(napi_get_cb_info(env, info, NULL, NULL, &jsthis, NULL));

  markov::distribution::weights *weights;
  NAPI_CHECK(napi_unwrap(env, jsthis, reinterpret_cast<void **>(&weights)));

  std::string res{};
  for (const auto &state : weights->states()) {
    uint32_t count = (*weights)[state];

    res += std::to_string(count);
    res += " ";
  }

  if (res.length()) {
    res.pop_back();
  }

  napi_value val;
  NAPI_CHECK(napi_create_string_utf8(env, res.c_str(), res.length(), &val));

  return val;
}

} // namespace node
} // namespace markov

markov::node::nref markov::node::define_distribution_weight(napi_env env) {
  napi_property_descriptor dist_props[] = {
      DECLARE_NAPI_METHOD("insert", dist_weight_insert),
      DECLARE_NAPI_METHOD("toString", dist_weight_toString),
  };

  napi_value dist_cons;
  NAPI_CHECK(napi_define_class(env, "Weights", NAPI_AUTO_LENGTH,
                               dist_weight_ctor, NULL, 2, dist_props,
                               &dist_cons));

  napi_ref dist_ref;
  napi_create_reference(env, dist_cons, 1, &dist_ref);

  return markov::node::nref{dist_ref, dist_cons};
}
