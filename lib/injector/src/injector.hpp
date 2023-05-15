#pragma once
#include <Windows.h>
#include <string>
#include <optional>

class Injector 
{
public:
    std::optional<DWORD> find_process_id(const std::string& process_name);
    std::optional<std::string> inject(const std::string& process_name, const std::string& dll_path);
};
