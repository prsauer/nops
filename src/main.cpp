#include <napi.h>
#include <windows.h>
#include <psapi.h>
#include "utils.h"
#include "wincalls.h"
#include <vector>
#include <string>

int g_logLevel = 0; // Default log level to no logs

Napi::Value GetAllProcessNames(const Napi::CallbackInfo &info) {
  std::vector<std::string> processNames(1024);
  int vecIdx = 0;
  DWORD processes[1024];
  DWORD bytesNeeded;
  EnumProcesses(processes, sizeof(processes), &bytesNeeded);
  for (DWORD i = 0; i < bytesNeeded / sizeof(DWORD); i++) {
    HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, processes[i]);
    if (hProcess) {
      DWORD dwFlags = 0;        // Use win32 path format
      CHAR imagePath[MAX_PATH]; // LPSTR can be defined as CHAR, MAX_PATH is 260 bytes
      DWORD imageSize = MAX_PATH;
      BOOL imageName = QueryFullProcessImageNameA(hProcess, dwFlags, imagePath, &imageSize);
      processNames[vecIdx] = imagePath;
      vecIdx++;
      CloseHandle(hProcess);
    }
  }
  Napi::Array result = Napi::Array::New(info.Env(), vecIdx);
  for (size_t i = 0; i < vecIdx; ++i) {
    result[i] = Napi::String::New(info.Env(), processNames[i]);
  }
  return result;
}

Napi::Value GetAppWindowNames(const Napi::CallbackInfo &info) {
  blog(LOG_INFO, "Enumerating all main application windows...");

  std::vector<WindowInfo> windowInfos;

  // Enumerate all top-level windows
  EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&windowInfos));

  blog(LOG_INFO, "Window enumeration complete");

  Napi::Env env = info.Env();
  Napi::Array result = Napi::Array::New(env, windowInfos.size());
  for (size_t i = 0; i < windowInfos.size(); ++i) {
    Napi::Object windowObject = Napi::Object::New(env);
    windowObject.Set("executable", Napi::String::New(env, windowInfos[i].executable));
    windowObject.Set("title", Napi::String::New(env, windowInfos[i].title));
    result[i] = windowObject;
  }
  return result;
}

Napi::Value GetAllWindowNames(const Napi::CallbackInfo &info) {
  blog(LOG_INFO, "Enumerating ALL visible windows (verbose mode)...");

  std::vector<WindowInfo> windowInfos;

  // Enumerate all windows with minimal filtering
  EnumWindows(EnumAllWindowsProc, reinterpret_cast<LPARAM>(&windowInfos));

  blog(LOG_INFO, "Verbose window enumeration complete");

  Napi::Env env = info.Env();
  Napi::Array result = Napi::Array::New(env, windowInfos.size());
  for (size_t i = 0; i < windowInfos.size(); ++i) {
    Napi::Object windowObject = Napi::Object::New(env);
    windowObject.Set("executable", Napi::String::New(env, windowInfos[i].executable));
    windowObject.Set("title", Napi::String::New(env, windowInfos[i].title));
    result[i] = windowObject;
  }
  return result;
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
  exports.Set("SetLogLevel", Napi::Function::New(env, SetLogLevel));

  return exports;
}

NODE_API_MODULE(addon, Init)
