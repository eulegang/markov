#include "napi.h"
#include <node/js_native_api.h>
#include <node/js_native_api_types.h>

#define CHECK(expr)                                                            \
  if (napi_status status = (expr); status != napi_ok)                          \
  throw markov::node::status_exception(status)

napi_value markov::node::napi::new_target(napi_callback_info info) {
  napi_value target;
  CHECK(napi_get_new_target(_env, info, &target));
  return target;
}

void markov::node::napi::throw_error(const char *msg) {
  CHECK(napi_throw_error(_env, NULL, msg));
}

void markov::node::napi::cb_info(napi_callback_info info, size_t *size,
                                 napi_value *argv, napi_value *jsthis,
                                 void **oob) {

  CHECK(napi_get_cb_info(_env, info, size, argv, jsthis, oob));
}

bool markov::node::napi::is_array(napi_value value) {
  bool res;
  CHECK(napi_is_array(_env, value, &res));
  return res;
}

uint32_t markov::node::napi::array_length(napi_value value) {
  uint32_t len;
  CHECK(napi_get_array_length(_env, value, &len));
  return len;
}

napi_value markov::node::napi::array_elem(napi_value arr, uint32_t i) {
  napi_value val;
  CHECK(napi_get_element(_env, arr, i, &val));
  return val;
}

napi_valuetype markov::node::napi::typeof(napi_value val) {
  napi_valuetype ty;
  CHECK(napi_typeof(_env, val, &ty));
  return ty;
}

uint32_t markov::node::napi::get_uint32(napi_value val) {
  uint32_t i;
  CHECK(napi_get_value_uint32(_env, val, &i));
  return i;
}

void markov::node::napi::wrap(napi_value jsthis, void *data,
                              napi_finalize dtor) {
  CHECK(napi_wrap(_env, jsthis, data, dtor, NULL, NULL));
}

void markov::node::napi::unwrap(napi_value jsthis, void **data) {
  CHECK(napi_unwrap(_env, jsthis, data));
}

void markov::node::napi::set_prop(napi_value obj, const char *name,
                                  napi_value val) {

  CHECK(napi_set_named_property(_env, obj, name, val));
}

napi_value markov::node::napi::create_utf8(std::string &str) {
  napi_value value;
  CHECK(napi_create_string_utf8(_env, str.c_str(), str.length(), &value));
  return value;
}

napi_value markov::node::napi::create_uint32(uint32_t val) {
  napi_value value;

  CHECK(napi_create_uint32(_env, val, &value));

  return value;
}

napi_value markov::node::napi::create_uint64(uint64_t val) {
  napi_value value;

  CHECK(napi_create_bigint_uint64(_env, val, &value));

  return value;
}

bool markov::node::napi::instanceof(napi_value val, napi_value cons) {
  bool result;

  napi_valuetype ty = typeof(val);
  (void)ty;
  CHECK(napi_instanceof(_env, val, cons, &result));
  return result;
}

bool markov::node::napi::instanceof(napi_value val, napi_ref cons) {

  return instanceof(val, deref(cons));
}

napi_value markov::node::napi::new_instance(napi_value cons, size_t argc,
                                            napi_value *argv) {
  napi_value val;
  CHECK(napi_new_instance(_env, cons, argc, argv, &val));
  return val;
}

napi_value markov::node::napi::new_instance(napi_ref cons, size_t argc,
                                            napi_value *argv) {
  return new_instance(deref(cons), argc, argv);
}

napi_value markov::node::napi::deref(napi_ref ref) {
  napi_value res;
  napi_get_reference_value(_env, ref, &res);

  return res;
}

napi_value markov::node::napi::create_f64(double d) {
  napi_value value;
  CHECK(napi_create_double(_env, d, &value));

  return value;
}

napi_ref markov::node::napi::ref(napi_value val) {
  napi_ref reference;
  CHECK(napi_create_reference(_env, val, 1, &reference));
  return reference;
}

markov::node::status_exception::status_exception(napi_status status)
    : _status{status} {
  switch (status) {
  case napi_ok:
    _msg = "ok";
    break;
  case napi_invalid_arg:
    _msg = "invalid_arg";
    break;
  case napi_object_expected:
    _msg = "object_expected";
    break;
  case napi_string_expected:
    _msg = "string_expected";
    break;
  case napi_name_expected:
    _msg = "name_expected";
    break;
  case napi_function_expected:
    _msg = "function_expected";
    break;
  case napi_number_expected:
    _msg = "number_expected";
    break;
  case napi_boolean_expected:
    _msg = "boolean_expected";
    break;
  case napi_array_expected:
    _msg = "array_expected";
    break;
  case napi_generic_failure:
    _msg = "generic_failure";
    break;
  case napi_pending_exception:
    _msg = "pending_exception";
    break;
  case napi_cancelled:
    _msg = "cancelled";
    break;
  case napi_escape_called_twice:
    _msg = "escape_called_twice";
    break;
  case napi_handle_scope_mismatch:
    _msg = "handle_scope_mismatch";
    break;
  case napi_callback_scope_mismatch:
    _msg = "callback_scope_mismatch";
    break;
  case napi_queue_full:
    _msg = "queue_full";
    break;
  case napi_closing:
    _msg = "closing";
    break;
  case napi_bigint_expected:
    _msg = "bigint_expected";
    break;
  case napi_date_expected:
    _msg = "date_expected";
    break;
  case napi_arraybuffer_expected:
    _msg = "arraybuffer_expected";
    break;
  case napi_detachable_arraybuffer_expected:
    _msg = "detachable_arraybuffer_expected";
    break;
  case napi_would_deadlock:
    _msg = "would_deadlock";
    break; // unused
  case napi_no_external_buffers_allowed:
    _msg = "no_external_buffers_allowed";
    break;
  case napi_cannot_run_js:
    _msg = "cannot_run_js";
    break;
  }
}
