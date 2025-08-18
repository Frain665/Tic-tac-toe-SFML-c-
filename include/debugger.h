#pragma once

#include <iostream>
#include <string>
#include <sstream>

// ������� ��� �������
#ifdef DEBUG_BUILD
#define DEBUG_LOG(msg) Debugger::log(msg)
#define DEBUG_LOG_FORMAT(format, ...) Debugger::logFormat(format, __VA_ARGS__)
#define DEBUG_ASSERT(condition, msg) \
        do { \
            if (!(condition)) { \
                Debugger::log("ASSERTION FAILED: " + std::string(msg)); \
                std::abort(); \
            } \
        } while(0)
#else
#define DEBUG_LOG(msg) ((void)0)
#define DEBUG_LOG_FORMAT(format, ...) ((void)0)
#define DEBUG_ASSERT(condition, msg) ((void)0)
#endif

class Debugger {
public:
    // ����������� ������ ��� �����������
    static void log(const std::string& message);
    static void logFormat(const std::string& format, ...);
    static void logError(const std::string& message);
    static void logWarning(const std::string& message);

    // ��������� ������ ����������� �����
    enum LogLevel {
        NONE = 0,
        ERROR = 1,
        WARNING = 2,
        INFO = 3,
        DEBUG = 4
    };

    static void setLogLevel(LogLevel level);
    static LogLevel getLogLevel();

private:
    static LogLevel currentLogLevel;
    static std::string getCurrentTime();
};