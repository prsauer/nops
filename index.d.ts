/**
 * Represents information about a window.
 */
export interface WindowInfo {
  /**
   * The name of the executable that owns the window.
   */
  executable: string
  /**
   * The title of the window.
   */
  title: string
}

/**
 * Retrieves a list of all running process executable names.
 * @returns A promise that resolves to an array of strings, where each string is the full path to a running process.
 */
export function GetAllProcessNames(): Promise<string[]>

/**
 * Retrieves information about all main application windows.
 * @returns A promise that resolves to an array of WindowInfo objects for each main application window.
 */
export function GetAppWindowNames(): Promise<WindowInfo[]>

/**
 * Retrieves information about all visible windows.
 * @returns A promise that resolves to an array of WindowInfo objects for each visible window.
 */
export function GetAllWindowNames(): Promise<WindowInfo[]>

/**
 * Sets the global log level for the native module. Logs to console.
 * Default is 0 (no logs).
 * @param level - The log level to set.
 * 100: LOG_ERROR
 * 200: LOG_WARNING
 * 300: LOG_INFO
 * 400: LOG_DEBUG
 * Any other value will disable logging.
 */
export function SetLogLevel(level: number): void
