#pragma once

#include <windows.h>
#include <string>
#include <vector>

DWORD GetProcessIdByName(const std::wstring &processName);
std::vector<int> GetUdpPortsForProcess(DWORD processId);
