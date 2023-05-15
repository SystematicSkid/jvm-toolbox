#include "injector.hpp"
#include <windows.h>
#include <psapi.h>
#include <format>
#include <iostream>
#include <winternl.h>
#include <vector>

/* ntdll for NtQueryInfo */
#pragma comment(lib, "ntdll.lib")

std::optional<std::wstring> Injector::get_process_command_line(HANDLE process)
{
    ULONG size = sizeof(PROCESS_BASIC_INFORMATION);
    PROCESS_BASIC_INFORMATION pbi;

    if (NtQueryInformationProcess(process, ProcessBasicInformation, &pbi, size, &size) != 0)
    {
        std::cerr << "Failed to get process basic information\n";
        return std::nullopt;
    }

    PEB peb;
    SIZE_T bytes_read;

    if (!ReadProcessMemory(process, pbi.PebBaseAddress, &peb, sizeof(peb), &bytes_read))
    {
        std::cerr << "Failed to read peb address\n";
        return std::nullopt;
    }

    RTL_USER_PROCESS_PARAMETERS upp;
    if (!ReadProcessMemory(process, peb.ProcessParameters, &upp, sizeof(upp), &bytes_read))
    {
        std::cerr << "Failed to read process parameters\n";
        return std::nullopt;
    }

    std::vector<wchar_t> command_line(upp.CommandLine.Length);
    if (!ReadProcessMemory(process, upp.CommandLine.Buffer, command_line.data(), upp.CommandLine.Length, &bytes_read))
    {
        std::cerr << "Failed to read command line memory\n";
        return std::nullopt;
    }

    return std::wstring(command_line.begin(), command_line.end());
}

std::optional<DWORD> Injector::find_process_id(const std::string &process_name, const std::string &java_name)
{
    DWORD process_ids[1024], bytes_returned;

    if (!EnumProcesses(process_ids, sizeof(process_ids), &bytes_returned))
    {
        std::cerr << "Failed to enumerate processes\n";
        return std::nullopt;
    }

    char base_name[MAX_PATH];
    for (unsigned int i = 0; i < bytes_returned / sizeof(DWORD); ++i)
    {
        HANDLE process = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, process_ids[i]);
        if (process == nullptr)
        {
            continue;
        }

        HMODULE module;
        DWORD bytes_needed;
        if (EnumProcessModules(process, &module, sizeof(module), &bytes_needed))
        {
            GetModuleBaseName(process, module, base_name, sizeof(base_name) / sizeof(char));

            if (process_name == base_name)
            {
                auto cmd_line = get_process_command_line(process);
                if (cmd_line.has_value())
                {
                    std::wstring command_line = cmd_line.value();
                    std::size_t classpath_pos = command_line.find(L"-classpath");
                    std::size_t jar_pos = command_line.find(L"-jar");

                    if (classpath_pos == std::wstring::npos && jar_pos == std::wstring::npos)
                    {
                        std::cerr << "Neither -classpath nor -jar found in command line of process " << process_ids[i] << "\n";
                        continue;
                    }

                    std::size_t start_pos = classpath_pos != std::wstring::npos ? classpath_pos : jar_pos;
                    std::size_t end_pos = command_line.find(L".jar", start_pos);
                    end_pos = end_pos == std::wstring::npos ? command_line.find(L".class", start_pos) : end_pos;

                    if (end_pos == std::wstring::npos)
                    {
                        std::cerr << "Neither .jar nor .class found in command line of process " << process_ids[i] << "\n";
                        continue;
                    }

                    std::wstring classpath = command_line.substr(start_pos, end_pos - start_pos);
                    std::wstring java_name_wstr(java_name.begin(), java_name.end());

                    if (classpath.find(java_name_wstr) == std::wstring::npos)
                    {
                        std::cerr << "Java name " << java_name << " not found in classpath of process " << process_ids[i] << "\n";
                        continue;
                    }

                    std::wcout << L"Found process with classpath: " << classpath << '\n';
                    CloseHandle(process);
                    return process_ids[i];
                }
                else
                {
                    std::cerr << "Failed to retrieve command line of process " << process_ids[i] << "\n";
                    continue;
                }
            }
        }
        else
        {
            std::cerr << "Failed to enumerate process modules for process " << process_ids[i] << "\n";
        }

        CloseHandle(process);
    }

    return std::nullopt;
}

std::optional<std::string> Injector::inject(const std::string &process_name, const std::string &java_name, const std::string &dll_path)
{
    auto pid_opt = find_process_id(process_name, java_name);
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
