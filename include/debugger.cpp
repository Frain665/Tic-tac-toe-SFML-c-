#include "debugger.h"
#include <cstdarg>
#include <chrono>
#include <iomanip>

// Инициализация статической переменной
Debugger::LogLevel Debugger::currentLogLevel = Debugger::LogLevel::INFO;

void Debugger::log(const std::string& message) {
    if (currentLogLevel >= INFO) {
        std::cout << "[" << getCurrentTime() << "] INFO: " << message << std::endl;
    }
}

void Debugger::logFormat(const std::string& format, ...) {
    if (currentLogLevel >= INFO) {
        char buffer[1024];
        va_list args;
        va_start(args, format);
        vsnprintf(buffer, sizeof(buffer), format.c_str(), args);
        va_end(args);

        std::cout << "[" << getCurrentTime() << "] INFO: " << buffer << std::endl;
    }
}

void Debugger::logError(const std::string& message) {
    if (currentLogLevel >= ERROR) {
        std::cerr << "[" << getCurrentTime() << "] ERROR: " << message << std::endl;
    }
}

void Debugger::logWarning(const std::string& message) {
    if (currentLogLevel >= WARNING) {
        std::cout << "[" << getCurrentTime() << "] WARNING: " << message << std::endl;
    }
}

void Debugger::setLogLevel(LogLevel level) {
    currentLogLevel = level;
}

Debugger::LogLevel Debugger::getLogLevel() {
    return currentLogLevel;
}

std::string Debugger::getCurrentTime() {
    auto now = std::chrono::system_clock::now();
    auto time_t = std::chrono::system_clock::to_time_t(now);
    auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
        now.time_since_epoch()) % 1000;

    std::stringstream ss;
    ss << std::put_time(std::localtime(&time_t), "%H:%M:%S");
    ss << '.' << std::setfill('0') << std::setw(3) << ms.count();

    return ss.str();
}