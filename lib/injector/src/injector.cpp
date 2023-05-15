#include "injector.hpp"
#include <windows.h>
#include <psapi.h>
#include <format>

std::optional<DWORD> Injector::find_process_id(const std::string &process_name)
{
    DWORD process_ids[1024], bytes_returned;

    if (!EnumProcesses(process_ids, sizeof(process_ids), &bytes_returned))
    {
        return std::nullopt;
    }

    char base_name[MAX_PATH];
    for (unsigned int i = 0; i < bytes_returned / sizeof(DWORD); ++i)
    {
        HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process_ids[i]);
        if (process != nullptr)
        {
            HMODULE module;
            DWORD bytes_needed;
            if (EnumProcessModules(process, &module, sizeof(module), &bytes_needed))
            {
                GetModuleBaseName(process, module, base_name, sizeof(base_name) / sizeof(char));
                if (process_name == base_name)
                {
                    CloseHandle(process);
                    return process_ids[i];
                }
            }
        }
        CloseHandle(process);
    }

    return std::nullopt;
}

std::optional<std::string> Injector::inject(const std::string &process_name, const std::string &dll_path)
{
    auto pid_opt = find_process_id(process_name);
    if (!pid_opt.has_value())
    {
        return std::format("Failed to find process with name: {}", process_name);
    }

    DWORD pid = pid_opt.value();
    HANDLE process = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
    if (process == nullptr)
    {
        return "Failed to open target process!";
    }

    void *loc = VirtualAllocEx(process, 0, MAX_PATH, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (loc == nullptr)
    {
        return "Failed to allocate memory in target process";
    }

    if (!WriteProcessMemory(process, loc, dll_path.c_str(), dll_path.size() + 1, 0))
    {
        return "Failed to write into target process's address space!";
    }

    HANDLE thread_id = CreateRemoteThread(process, 0, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA"), loc, 0, 0);
    if (thread_id == nullptr)
    {
        return "Failed to create remote thread in target process";
    }

    CloseHandle(process);
    return std::nullopt;
}
