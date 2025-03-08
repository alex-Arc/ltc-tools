#include "ltcwrapper.h"

#include "../node_modules/node-addon-api/napi.h"

Napi::Value ParseLtcChunk(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();

  Napi::Number result = Napi::Number::New(env, 123456789);
  return result;
};
