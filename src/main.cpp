#include <napi.h>
#include <windows.h>
#include <psapi.h>
#include "utils.h"
#include <cstdarg>
#include <cstdio>
#include <vector>
#include <string>

int g_logLevel = 0; // Default log level to no logs

void blog(int level, const char *format, ...) {
  if (level > g_logLevel) {
    return;
  }
  const char *levelStr = "";
  switch (level) {
  case LOG_INFO:
    levelStr = "INFO";
    break;
  case LOG_WARNING:
    levelStr = "WARN";
    break;
  case LOG_ERROR:
    levelStr = "ERROR";
    break;
  case LOG_DEBUG:
    levelStr = "DEBUG";
    break;
  }

  if (*levelStr) {
    printf("[%s] ", levelStr);
  }

  va_list args;
  va_start(args, format);
  vprintf(format, args);
  va_end(args);

  printf("\n");
}

// Callback for showing ALL windows (minimal filtering)
BOOL CALLBACK EnumAllWindowsProc(HWND hwnd, LPARAM lParam) {
  // Only filter out completely invisible windows
  if (!IsWindowVisible(hwnd)) {
    return TRUE;
  }

  // Get window title
  char windowTitle[256];
  int titleLength = GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle));

  // Get window class name
  char className[256];
  GetClassNameA(hwnd, className, sizeof(className));

  // Get process info
  DWORD processId;
  GetWindowThreadProcessId(hwnd, &processId);

  // Show window info even if no title
  const char *title = (titleLength > 0) ? windowTitle : "<No Title>";

  if (lParam) {
    std::vector<std::string> *windowNames = reinterpret_cast<std::vector<std::string> *>(lParam);
    windowNames->push_back(title);
  }

  // Check if it's a child window
  HWND parent = GetParent(hwnd);
  const char *parentInfo = (parent != NULL) ? " [Child]" : "";

  // Check window styles
  LONG_PTR exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
  const char *toolWindow = (exStyle & WS_EX_TOOLWINDOW) ? " [Tool]" : "";

  blog(LOG_INFO, "Window: '%s' [Class: %s] [PID: %d]%s%s", title, className, processId, parentInfo, toolWindow);

  return TRUE;
}

// Helper function to check if a window is a main application window
BOOL IsMainWindow(HWND hwnd) {
  // Must be visible
  if (!IsWindowVisible(hwnd)) {
    return FALSE;
  }

  // Skip child windows (we only want top-level windows)
  if (GetParent(hwnd) != NULL) {
    return FALSE;
  }

  // Skip windows that are owned by other windows (like dialogs)
  if (GetWindow(hwnd, GW_OWNER) != NULL) {
    return FALSE;
  }

  // Check window styles - skip tool windows and other non-main windows
  LONG_PTR style = GetWindowLongPtr(hwnd, GWL_STYLE);
  LONG_PTR exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);

  // Skip if it's a tool window
  if (exStyle & WS_EX_TOOLWINDOW) {
    return FALSE;
  }

  // Must have minimize/maximize buttons or be a dialog (typical main windows)
  if (!(style & WS_MINIMIZEBOX) && !(style & WS_MAXIMIZEBOX) && !(style & DS_MODALFRAME)) {
    return FALSE;
  }

  return TRUE;
}

// Callback function for EnumWindows with better filtering
BOOL CALLBACK EnumWindowsProc(HWND hwnd, LPARAM lParam) {
  // Apply main window filter
  if (!IsMainWindow(hwnd)) {
    return TRUE; // Continue enumeration
  }

  // Get window title
  char windowTitle[256];
  int titleLength = GetWindowTextA(hwnd, windowTitle, sizeof(windowTitle));

  // Skip windows without titles
  if (titleLength == 0) {
    return TRUE;
  }

  if (lParam) {
    std::vector<std::string> *windowNames = reinterpret_cast<std::vector<std::string> *>(lParam);
    windowNames->push_back(windowTitle);
  }

  // Get window class name for additional info
  // char className[256];
  // GetClassNameA(hwnd, className, sizeof(className));

  // Get window rect to show size
  // RECT windowRect;
  // GetWindowRect(hwnd, &windowRect);
  // int width = windowRect.right - windowRect.left;
  // int height = windowRect.bottom - windowRect.top;

  // Skip very small windows (likely not real application windows)
  // if (width < 100 || height < 50) {
  //   return TRUE;
  // }

  // Get process info
  // DWORD processId;
  // GetWindowThreadProcessId(hwnd, &processId);

  blog(LOG_INFO, "Window: '%s'", windowTitle);

  return TRUE; // Continue enumeration
}

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

  std::vector<std::string> windowNames;

  // Enumerate all top-level windows
  EnumWindows(EnumWindowsProc, reinterpret_cast<LPARAM>(&windowNames));

  blog(LOG_INFO, "Window enumeration complete");

  Napi::Env env = info.Env();
  Napi::Array result = Napi::Array::New(env, windowNames.size());
  for (size_t i = 0; i < windowNames.size(); ++i) {
    result[i] = Napi::String::New(env, windowNames[i]);
  }
  return result;
}

Napi::Value GetAllWindowNames(const Napi::CallbackInfo &info) {
  blog(LOG_INFO, "Enumerating ALL visible windows (verbose mode)...");

  std::vector<std::string> windowNames;

  // Enumerate all windows with minimal filtering
  EnumWindows(EnumAllWindowsProc, reinterpret_cast<LPARAM>(&windowNames));

  blog(LOG_INFO, "Verbose window enumeration complete");

  Napi::Env env = info.Env();
  Napi::Array result = Napi::Array::New(env, windowNames.size());
  for (size_t i = 0; i < windowNames.size(); ++i) {
    result[i] = Napi::String::New(env, windowNames[i]);
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
