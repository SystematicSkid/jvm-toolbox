#pragma once
#include <cstdint>
#include <Windows.h>
#include <memory>
/* Types*/
#include "./types/types.hpp"
/* Interfaces */
#include "./interfaces/java_interface.hpp"

/* Utility */
#include "./utility/jni_errors.hpp"

/* IPC */
#include "ipc/include/ipc.hpp"
#include "ipc/include/messages/inspector_info_generated.h"

namespace inspector
{
    extern std::unique_ptr<interfaces::JavaInterface> java_interface;
    extern HANDLE inspector_module;

    extern std::unique_ptr<ipc::Producer> producer;
}