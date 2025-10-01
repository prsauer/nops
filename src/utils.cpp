#include <iostream>
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "utils.h"

std::string get_current_date_time() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%Y-%m-%d %H-%M-%S");
    return ss.str();
}
