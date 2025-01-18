#include "console.hpp"

void Console::Log(LogLevel Level, const std::string& Message)
{
    const char* Prefix = nullptr;

    switch (Level)
    {
    case LogLevel::Info: 
        Prefix = ("[i] "); 
        break;
    case LogLevel::Success: 
        Prefix = ("[+] "); 
        break;
    case LogLevel::Error:   
        Prefix = ("[-] "); 
        break;
    }
    std::cout << (Prefix) << Message << "\n";
}

void Console::LogDebug(const std::string& Name, uintptr_t Value)
{
    std::cout << ("[+] ") << Name << " -> " << Value << "\n";
}
