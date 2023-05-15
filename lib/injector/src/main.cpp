#include <Windows.h>
#include "injector.hpp"
#include <iostream>
#include <format>

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cout << "Usage: Injector.exe [Process Name] [Class or Jar Name] [Path to DLL]\n";
        return 1;
    }

    Injector injector;

    std::string process_name = argv[1];
    std::string java_name = argv[2];
    std::string dll_path = argv[3];

    auto result = injector.inject(process_name, java_name, dll_path);
    if (result.has_value())
    {
        std::cerr << result.value() << '\n';
        return 1;
    }

    std::cout << "DLL Injected successfully\n";

    return 0;
}
