#include <string>
#include <iostream>

#ifndef CONSOLE_HPP
#define CONSOLE_HPP

enum class LogLevel
{
    Info,
    Success,
    Error
};

class Console
{
public:
    void Log(LogLevel Level, const std::string& Message);
    void LogDebug(const std::string& Name, uintptr_t Value);
}; static Console* console = new Console;

#endif // CONSOLE_HPP