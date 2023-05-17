#include <Windows.h>
#include "injector.hpp"
#include <iostream>
#include <format>
#include "ipc/include/ipc.hpp"
#include "ipc/include/messages/inspector_info_generated.h"

std::atomic<bool> is_injected = false;

int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        std::cout << "Usage: Injector.exe [Process Name] [Class or Jar Name] [Path to DLL]\n";
        return 1;
    }
    /* Setup consumer */
    ipc::Consumer consumer("inspector", 1024 * 1024,
        [](const std::vector<unsigned char>& message)
        {
            auto data = flatbuffers::GetRoot<jvm_toolbox_flatbuffers::inspector::OnInspectorLoaded>(message.data());
            printf("Interface: %s\n", data->inspector_interface()->str().c_str());
            /* Safe set 'is_injected' to true */
            is_injected.store(true, std::memory_order_release);
        });

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

    /* Wait for 'is_injected' */
    while (!is_injected.load(std::memory_order_acquire))
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    std::cout << "DLL Injected successfully\n";

    return 0;
}
