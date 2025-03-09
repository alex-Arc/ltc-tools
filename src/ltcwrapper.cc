#include <napi.h>
#include <ltc.h>
#include "ltcwrapper.h"

using namespace Napi;

#define LTC_QUEUE_LENGTH 16
#define BUFFER_SIZE 1024
#define TIME_CODE_STRING_SIZE 12
ltcsnd_sample_t sound[BUFFER_SIZE];

size_t n;
long long int total = 0;
int expected_fps = 0;

LTCDecoder *decoder;
LTCFrameExt frame;

int fps_den = 1;
int fps_num = 25;
int samplerate = 48000;

long int ltc_frame_length_samples = samplerate * fps_den / fps_num;
long int ltc_frame_length_fudge = (ltc_frame_length_samples * 101 / 100);

long int prev_read = ltc_frame_length_samples;
LTCFrameExt prev_frame;

Value Fn(const CallbackInfo &info)
{
  Env env = info.Env();

  // ltc_decoder_write(decoder,);

  return String::New(env, "Hello World");
};

Object Init(Env env, Object exports)
{
  decoder = ltc_decoder_create(samplerate * fps_den / fps_num, LTC_QUEUE_LENGTH);
  exports.Set(String::New(env, "fn"), Function::New<Fn>(env));
  return exports;
}

NODE_API_MODULE(ltc - tools, Init)