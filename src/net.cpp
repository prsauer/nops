#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "net.h"
#include "wincalls.h"
#include "utils.h"
#include <tlhelp32.h>
#include <iphlpapi.h>
#include <vector>
#include <winsock2.h>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "ws2_32.lib")

DWORD GetProcessIdByName(const std::wstring &processName) {
  PROCESSENTRY32W entry;
  entry.dwSize = sizeof(PROCESSENTRY32W);

  HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
  if (snapshot == INVALID_HANDLE_VALUE) {
    blog(LOG_ERROR, "Failed to create toolhelp snapshot");
    return 0;
  }

  if (Process32FirstW(snapshot, &entry) == TRUE) {
    while (Process32NextW(snapshot, &entry) == TRUE) {
      if (std::wstring(entry.szExeFile) == processName) {
        CloseHandle(snapshot);
        return entry.th32ProcessID;
      }
    }
  }

  CloseHandle(snapshot);
  return 0;
}

std::vector<int> GetUdpPortsForProcess(DWORD processId) {
  std::vector<int> ports;
  if (processId == 0) {
    return ports;
  }

  DWORD udpTableSize = 0;
  if (GetExtendedUdpTable(NULL, &udpTableSize, FALSE, AF_INET, UDP_TABLE_OWNER_PID, 0) != ERROR_INSUFFICIENT_BUFFER) {
    blog(LOG_ERROR, "GetExtendedUdpTable failed to get size");
    return ports;
  }

  std::vector<BYTE> udpTableBuffer(udpTableSize);
  PMIB_UDPTABLE_OWNER_PID pUdpTable = reinterpret_cast<PMIB_UDPTABLE_OWNER_PID>(udpTableBuffer.data());

  if (GetExtendedUdpTable(pUdpTable, &udpTableSize, FALSE, AF_INET, UDP_TABLE_OWNER_PID, 0) != NO_ERROR) {
    blog(LOG_ERROR, "GetExtendedUdpTable failed to get table");
    return ports;
  }

  for (DWORD i = 0; i < pUdpTable->dwNumEntries; i++) {
    if (pUdpTable->table[i].dwOwningPid == processId) {
      ports.push_back(ntohs(static_cast<u_short>(pUdpTable->table[i].dwLocalPort)));
    }
  }

  return ports;
}
