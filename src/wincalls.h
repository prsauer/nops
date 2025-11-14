#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#pragma once

#include <string>

struct WindowInfo {
  std::string executable;
  std::string title;
};

extern int g_logLevel;

void blog(int level, const char *format, ...);
std::string GetProcessExeName(DWORD processId);
BOOL IsMainWindow(HWND hwnd);
BOOL CALLBACK EnumAllWindowsProc(HWND hwnd, LPARAM lParam);
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam);
