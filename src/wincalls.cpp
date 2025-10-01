#include <napi.h>
#include <windows.h>
#include <psapi.h>
#include "utils.h"
#include "wincalls.h"
#include <cstdarg>
#include <cstdio>
#include <vector>
#include <string>

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

std::string GetProcessExeName(DWORD processId) {
  HANDLE hProcess = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION, FALSE, processId);
  if (hProcess) {
    CHAR imagePath[MAX_PATH];
    DWORD imageSize = MAX_PATH;
    if (QueryFullProcessImageNameA(hProcess, 0, imagePath, &imageSize)) {
      CloseHandle(hProcess);
      std::string fullPath(imagePath);
      size_t lastBackslash = fullPath.find_last_of("\\");
      if (std::string::npos != lastBackslash) {
        return fullPath.substr(lastBackslash + 1);
      }
      return fullPath;
    }
    CloseHandle(hProcess);
  }
  return "<unknown>";
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
  std::string exeName = GetProcessExeName(processId);

  // Show window info even if no title
  const char *title = (titleLength > 0) ? windowTitle : "<No Title>";

  if (lParam) {
    std::vector<WindowInfo> *windowInfos = reinterpret_cast<std::vector<WindowInfo> *>(lParam);
    windowInfos->push_back({exeName, title});
  }

  // Check if it's a child window
  HWND parent = GetParent(hwnd);
  const char *parentInfo = (parent != NULL) ? " [Child]" : "";

  // Check window styles
  LONG_PTR exStyle = GetWindowLongPtr(hwnd, GWL_EXSTYLE);
  const char *toolWindow = (exStyle & WS_EX_TOOLWINDOW) ? " [Tool]" : "";

  blog(LOG_INFO, "Window: '%s' [Executable: %s] [Class: %s] [PID: %d]%s%s", title, exeName.c_str(), className,
       processId, parentInfo, toolWindow);

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

  // Commented out: Games often don't have these
  // Must have minimize/maximize buttons or be a dialog (typical main windows)
  //   if (!(style & WS_MINIMIZEBOX) && !(style & WS_MAXIMIZEBOX) && !(style & DS_MODALFRAME)) {
  //     return FALSE;
  //   }

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

  DWORD processId;
  GetWindowThreadProcessId(hwnd, &processId);
  std::string exeName = GetProcessExeName(processId);

  if (lParam) {
    std::vector<WindowInfo> *windowInfos = reinterpret_cast<std::vector<WindowInfo> *>(lParam);
    windowInfos->push_back({exeName, windowTitle});
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

  blog(LOG_INFO, "Window: '%s' [Executable: %s]", windowTitle, exeName.c_str());

  return TRUE; // Continue enumeration
}
