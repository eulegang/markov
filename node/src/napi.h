#ifndef _MARKOV_NAPI_H
#define _MARKOV_NAPI_H

#include <exception>
#include <node/js_native_api_types.h>
#include <node/node_api.h>
#include <string>

namespace markov {
namespace node {

class napi {
  napi_env _env;

public:
  napi(napi_env env) : _env{env} {};

  void cb_info(napi_callback_info, size_t *size, napi_value *argv,
               napi_value *jsthis = NULL, void **oob = NULL);
  void throw_error(const char *);
  napi_value new_target(napi_callback_info info);
  bool is_array(napi_value value);
  uint32_t array_length(napi_value value);
  napi_value array_elem(napi_value arr, uint32_t i);

  napi_valuetype typeof(napi_value);
  uint32_t get_uint32(napi_value);

  void wrap(napi_value jsthis, void *data, napi_finalize dtor);
  void unwrap(napi_value jsthis, void **data);
  template <typename T> inline T *unwrap(napi_value value) {
    T *res;
    unwrap(value, reinterpret_cast<void **>(&res));
    return res;
  }

  void set_prop(napi_value obj, const char *name, napi_value val);

  napi_value create_utf8(std::string &str);
  napi_value create_f64(double d);
  napi_value create_uint32(uint32_t t);
  napi_value create_uint64(uint64_t t);

  napi_value deref(napi_ref ref);
  napi_ref ref(napi_value);
  bool instanceof(napi_value value, napi_value cons);
  bool instanceof(napi_value value, napi_ref cons);
  napi_value new_instance(napi_value cons, size_t argc, napi_value *argv);
  napi_value new_instance(napi_ref cons, size_t argc, napi_value *argv);
};

class status_exception : public std::exception {
  std::string _msg;
  napi_status _status;

public:
  status_exception(napi_status status);
  napi_status status() const { return _status; }

  const char *what() const noexcept override { return _msg.c_str(); }
}; // namespace node

} // namespace node
} // namespace markov

#endif
