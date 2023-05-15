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

namespace inspector
{
    extern std::unique_ptr<interfaces::JavaInterface> java_interface;
    extern HANDLE inspector_module;
}