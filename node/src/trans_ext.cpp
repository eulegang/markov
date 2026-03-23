#include "extension.h"
#include "markov.h"
#include <cstdlib>
#include <format>
#include <node/js_native_api_types.h>

namespace markov {
namespace node {

napi_ref transition_cons;

void trans_dtor(napi_env, void *data, void *) {
  auto trans = static_cast<markov::transition *>(data);
  delete trans;
}

napi_value trans_ctor(napi_env env, napi_callback_info info) {
  markov::node::napi node{env};
  napi_value target = node.new_target(info);

  if (target == nullptr) {
    napi_throw_error(env, nullptr, "use new keyword");
    return NULL;
  }

  size_t argc = 1;
  napi_value jsthis;
  napi_value argv[1];

  node.cb_info(info, &argc, argv, &jsthis);

  if (argc < 1) {
    node.throw_error("invalid arguments");
    return NULL;
  }

  if (!node.instanceof(argv[0], node.deref(transition_weight_cons))) {
    node.throw_error("transition must be constructed with weights");
    return NULL;
  }

  markov::transition::weights *weights;
  node.unwrap(argv[0], reinterpret_cast<void **>(&weights));

  if (!weights->is_ready()) {
    node.throw_error("invalid weights");
    return NULL;
  }

  markov::transition *trans = new markov::transition{*weights};

  node.wrap(jsthis, trans, trans_dtor);

  return jsthis;
}

napi_value trans_toString(napi_env env, napi_callback_info info) {
  napi node{env};
  napi_value jsthis;
  node.cb_info(info, NULL, NULL, &jsthis);

  markov::transition *trans;
  node.unwrap(jsthis, reinterpret_cast<void **>(&trans));

  std::string res{};

  for (const auto &from : trans->states()) {
    for (const auto &to : trans->states()) {
      double perc = (*trans)[from, to];
      res += std::format("{:.2f}%", perc * 100.0);
      res += " ";
    }

    res.pop_back();
    res += "; ";
  }
  res.pop_back();
  res.pop_back();

  return node.create_utf8(res);
}

napi_value trans_get(napi_env env, napi_callback_info info) {

  markov::node::napi node{env};
  size_t argc = 2;
  napi_value argv[2];
  napi_value jsthis;

  node.cb_info(info, &argc, argv, &jsthis);

  if (argc != 2 || node.typeof(argv[0]) != napi_number ||
      node.typeof(argv[1]) != napi_number) {
    node.throw_error("invalid arguments");
    return NULL;
  }

  markov::transition *transition = node.unwrap<markov::transition>(jsthis);

  uint32_t row = node.get_uint32(argv[0]);
  uint32_t col = node.get_uint32(argv[1]);

  if (row >= transition->states().size() ||
      col >= transition->states().size()) {
    node.throw_error("invalid arguments");
    return NULL;
  }

  return node.create_f64((*transition)[row, col]);
}

napi_value trans_states(napi_env env, napi_callback_info info) {
  napi node{env};

  napi_value jsthis;

  node.cb_info(info, NULL, NULL, &jsthis);

  markov::transition *transition = node.unwrap<markov::transition>(jsthis);

  return node.create_uint32(transition->states().size());
}

napi_value trans_apply(napi_env env, napi_callback_info info) {
  markov::node::napi node{env};
  size_t argc = 1;
  napi_value argv[1];
  napi_value jsthis;

  node.cb_info(info, &argc, argv, &jsthis);

  if (argc < 1) {
    node.throw_error("invalid arguments");
    return NULL;
  }

  if (!node.instanceof(argv[0], node.deref(distribution_cons))) {
    node.throw_error("invalid argument");
    return NULL;
  }

  napi_value args[2] = {
      jsthis,
      argv[0],
  };

  return node.new_instance(distribution_cons, 2, args);
}

} // namespace node
} // namespace markov

napi_ref markov::node::define_transition(napi_env env) {
  markov::node::napi node{env};

  napi_property_descriptor trans_props[] = {
      DECLARE_NAPI_METHOD("toString", trans_toString),
      DECLARE_NAPI_METHOD("get", trans_get),
      DECLARE_NAPI_METHOD("apply", trans_apply),
      DECLARE_NAPI_GETTER("states", trans_states),
  };

  napi_value trans_cons;
  NAPI_CHECK(napi_define_class(env, "Transition", NAPI_AUTO_LENGTH, trans_ctor,
                               NULL, 4, trans_props, &trans_cons));

  transition_cons = node.ref(trans_cons);
  return transition_cons;
}
