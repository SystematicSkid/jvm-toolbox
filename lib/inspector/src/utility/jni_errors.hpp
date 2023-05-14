#pragma once
#include <unordered_map>

namespace inspector::utility
{
    extern std::unordered_map<std::int32_t, std::string> jvm_error_codes;
    extern std::unordered_map<std::int32_t, std::string> jvmti_error_codes;
}