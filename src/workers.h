#pragma once

#include <napi.h>
#include <vector>
#include "wincalls.h"

class AppWindowNamesWorker : public Napi::AsyncWorker {
public:
  AppWindowNamesWorker(Napi::Env env);
  ~AppWindowNamesWorker();
  void Execute() override;
  void OnOK() override;
  void OnError(const Napi::Error &e) override;
  Napi::Promise GetPromise();

private:
  Napi::Promise::Deferred deferred;
  std::vector<WindowInfo> windowInfos;
};

class AllWindowNamesWorker : public Napi::AsyncWorker {
public:
  AllWindowNamesWorker(Napi::Env env);
  ~AllWindowNamesWorker();
  void Execute() override;
  void OnOK() override;
  void OnError(const Napi::Error &e) override;
  Napi::Promise GetPromise();

private:
  Napi::Promise::Deferred deferred;
  std::vector<WindowInfo> windowInfos;
};

class ProcessNamesWorker : public Napi::AsyncWorker {
public:
  ProcessNamesWorker(Napi::Env env);
  ~ProcessNamesWorker();
  void Execute() override;
  void OnOK() override;
  void OnError(const Napi::Error &e) override;
  Napi::Promise GetPromise();

private:
  Napi::Promise::Deferred deferred;
  std::vector<std::string> processNames;
};

class UdpPortsWorker : public Napi::AsyncWorker {
public:
  UdpPortsWorker(Napi::Env &env, const std::string &processName);
  ~UdpPortsWorker() {};

  void Execute() override;
  void OnOK() override;

  Napi::Promise GetPromise();

private:
  Napi::Promise::Deferred m_deferred;
  std::string m_processName;
  std::vector<int> m_ports;
  bool m_processFound;
};
