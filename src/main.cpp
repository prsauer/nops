#include <napi.h>
#include "wincalls.h"
#include <psapi.h>
#include "workers.h"

int g_logLevel = 0; // Default log level to no logs

Napi::Value GetAllProcessNames(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  ProcessNamesWorker *worker = new ProcessNamesWorker(env);
  worker->Queue();
  return worker->GetPromise();
}

Napi::Value GetAppWindowNames(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  AppWindowNamesWorker *worker = new AppWindowNamesWorker(env);
  worker->Queue();
  return worker->GetPromise();
}

Napi::Value GetAllWindowNames(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  AllWindowNamesWorker *worker = new AllWindowNamesWorker(env);
  worker->Queue();
  return worker->GetPromise();
}

Napi::Value GetProcessUdpPorts(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1 || !info[0].IsString()) {
    Napi::TypeError::New(env, "String expected").ThrowAsJavaScriptException();
    return env.Undefined();
  }
  std::string processName = info[0].As<Napi::String>().Utf8Value();
  UdpPortsWorker *worker = new UdpPortsWorker(env, processName);
  worker->Queue();
  return worker->GetPromise();
}

Napi::Value SetLogLevel(const Napi::CallbackInfo &info) {
  Napi::Env env = info.Env();
  if (info.Length() < 1 || !info[0].IsNumber()) {
    Napi::TypeError::New(env, "Number expected").ThrowAsJavaScriptException();
    return env.Undefined();
  }
  g_logLevel = info[0].As<Napi::Number>().Int32Value();
  return env.Undefined();
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set("GetAllProcessNames", Napi::Function::New(env, GetAllProcessNames));
  exports.Set("GetAppWindowNames", Napi::Function::New(env, GetAppWindowNames));
  exports.Set("GetAllWindowNames", Napi::Function::New(env, GetAllWindowNames));
  exports.Set("GetProcessUdpPorts", Napi::Function::New(env, GetProcessUdpPorts));
  exports.Set("SetLogLevel", Napi::Function::New(env, SetLogLevel));

  return exports;
}

NODE_API_MODULE(addon, Init)
