#include "workers.h"
#include "wincalls.h"
#include "net.h"
#include <napi.h>
#include <windows.h>
#include <psapi.h>
#include <codecvt>
#include <locale>

// AppWindowNamesWorker implementation
AppWindowNamesWorker::AppWindowNamesWorker(Napi::Env env)
    : Napi::AsyncWorker(env), deferred(Napi::Promise::Deferred::New(env)) {}

AppWindowNamesWorker::~AppWindowNamesWorker() {}

void AppWindowNamesWorker::Execute() { EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&windowInfos)); }

void AppWindowNamesWorker::OnOK() {
  Napi::Env env = Env();
  Napi::HandleScope scope(env);

  Napi::Array result = Napi::Array::New(env, windowInfos.size());
  for (size_t i = 0; i < windowInfos.size(); ++i) {
    Napi::Object windowObject = Napi::Object::New(env);
    windowObject.Set("executable", Napi::String::New(env, windowInfos[i].executable));
    windowObject.Set("title", Napi::String::New(env, windowInfos[i].title));
    result[i] = windowObject;
  }

  deferred.Resolve(result);
}

void AppWindowNamesWorker::OnError(const Napi::Error &e) { deferred.Reject(e.Value()); }

Napi::Promise AppWindowNamesWorker::GetPromise() { return deferred.Promise(); }

// AllWindowNamesWorker implementation
AllWindowNamesWorker::AllWindowNamesWorker(Napi::Env env)
    : Napi::AsyncWorker(env), deferred(Napi::Promise::Deferred::New(env)) {}

AllWindowNamesWorker::~AllWindowNamesWorker() {}

void AllWindowNamesWorker::Execute() { EnumWindows(EnumAllWindowsProc, reinterpret_cast<LPARAM>(&windowInfos)); }

void AllWindowNamesWorker::OnOK() {
  Napi::Env env = Env();
  Napi::HandleScope scope(env);

  Napi::Array result = Napi::Array::New(env, windowInfos.size());
  for (size_t i = 0; i < windowInfos.size(); ++i) {
    Napi::Object windowObject = Napi::Object::New(env);
    windowObject.Set("executable", Napi::String::New(env, windowInfos[i].executable));
    windowObject.Set("title", Napi::String::New(env, windowInfos[i].title));
    result[i] = windowObject;
  }

  deferred.Resolve(result);
}

void AllWindowNamesWorker::OnError(const Napi::Error &e) { deferred.Reject(e.Value()); }

Napi::Promise AllWindowNamesWorker::GetPromise() { return deferred.Promise(); }

// ProcessNamesWorker implementation
ProcessNamesWorker::ProcessNamesWorker(Napi::Env env)
    : Napi::AsyncWorker(env), deferred(Napi::Promise::Deferred::New(env)) {}

ProcessNamesWorker::~ProcessNamesWorker() {}

void ProcessNamesWorker::Execute() {
  DWORD processes[1024];
  DWORD bytesNeeded;
  if (EnumProcesses(processes, sizeof(processes), &bytesNeeded)) {
    for (DWORD i = 0; i < bytesNeeded / sizeof(DWORD); i++) {
      if (processes[i] != 0) {
        std::string exeName = GetProcessExeName(processes[i]);
        processNames.push_back(exeName);
      }
    }
  }
}

void ProcessNamesWorker::OnOK() {
  Napi::Env env = Env();
  Napi::HandleScope scope(env);

  Napi::Array result = Napi::Array::New(env, processNames.size());
  for (size_t i = 0; i < processNames.size(); ++i) {
    result[i] = Napi::String::New(env, processNames[i]);
  }

  deferred.Resolve(result);
}

void ProcessNamesWorker::OnError(const Napi::Error &e) { deferred.Reject(e.Value()); }

Napi::Promise ProcessNamesWorker::GetPromise() { return deferred.Promise(); }

// UdpPortsWorker implementation
UdpPortsWorker::UdpPortsWorker(Napi::Env &env, const std::string &processName)
    : Napi::AsyncWorker(env), m_deferred(Napi::Promise::Deferred::New(env)), m_processName(processName),
      m_processFound(false) {}

void UdpPortsWorker::Execute() {
  std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
  std::wstring wideProcessName = converter.from_bytes(m_processName);

  DWORD processId = GetProcessIdByName(wideProcessName);
  if (processId != 0) {
    m_processFound = true;
    m_ports = GetUdpPortsForProcess(processId);
  } else {
    m_processFound = false;
  }
}

void UdpPortsWorker::OnOK() {
  Napi::Env env = Env();
  if (!m_processFound) {
    m_deferred.Resolve(env.Null());
  } else {
    Napi::Array arr = Napi::Array::New(env, m_ports.size());
    for (size_t i = 0; i < m_ports.size(); i++) {
      arr[i] = Napi::Number::New(env, m_ports[i]);
    }
    m_deferred.Resolve(arr);
  }
}

Napi::Promise UdpPortsWorker::GetPromise() { return m_deferred.Promise(); }
