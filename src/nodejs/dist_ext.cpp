#include "extension.h"
#include <node/js_native_api.h>

namespace markov {
namespace node {
napi_value dist_ctor(napi_env env, napi_callback_info info) {
  napi_value target;

  NAPI_CHECK(napi_get_new_target(env, info, &target));

  if (target != nullptr) {
    size_t argc = 1;
    napi_value jsthis;
    napi_value argv[1];
    NAPI_CHECK(napi_get_cb_info(env, info, &argc, argv, &jsthis, nullptr));

    bool is_array;
    NAPI_CHECK(napi_is_array(env, argv[0], &is_array));

    return NULL;
  } else {
    napi_throw_error(env, nullptr, "use new keyword");
    return NULL;
  }
}

napi_value dist_ratio(napi_env env, napi_callback_info info) {
  (void)env;
  (void)info;

  return NULL;
}
} // namespace node
} // namespace markov

markov::node::nref markov::node::define_distribution(napi_env env) {
  napi_property_descriptor dist_props[] = {
      DECLARE_NAPI_METHOD("ratio", dist_ratio),
  };

  napi_value dist_cons;
  NAPI_CHECK(napi_define_class(env, "Distribution", NAPI_AUTO_LENGTH, dist_ctor,
                               NULL, 1, dist_props, &dist_cons));

  napi_ref dist_ref;
  napi_create_reference(env, dist_cons, 1, &dist_ref);

  return markov::node::nref{dist_ref, dist_cons};
}
