#include "extension.h"
#include "markov.h"
#include <cstdlib>
#include <format>

namespace markov {
namespace node {

napi_ref transw_cons;

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

  if (!node.instanceof(argv[0], node.deref(transw_cons))) {
    node.throw_error("transition must be constructed with weights");
    return NULL;
  }

  markov::transition::weights *weights;
  node.unwrap(argv[0], reinterpret_cast<void **>(&weights));

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

} // namespace node
} // namespace markov

napi_ref markov::node::define_transition(napi_env env,
                                         napi_ref weight_cons_ref) {
  transw_cons = weight_cons_ref;

  markov::node::napi node{env};

  napi_property_descriptor trans_props[] = {
      DECLARE_NAPI_METHOD("toString", trans_toString),
  };

  napi_value trans_cons;
  NAPI_CHECK(napi_define_class(env, "Transition", NAPI_AUTO_LENGTH, trans_ctor,
                               NULL, 1, trans_props, &trans_cons));

  return node.ref(trans_cons);
}
