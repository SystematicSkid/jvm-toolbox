#pragma once
#include <Windows.h>
#include "base_event.hpp"

namespace inspector::events
{
    class ClassFileLoadEvent : public BaseEvent
    {
    private:
        std::unique_ptr<inspector::interfaces::JavaInterface> _java_interface;
    public:
        bool setup( std::unique_ptr<inspector::interfaces::JavaInterface> java_interface ) override;
        std::string get_name( ) override { return "ClassFileLoadEvent"; }
        bool enable( ) override;
        bool disable( ) override;
    private:
        /* Our event */
        static void class_file_load_hook(
            PVOID jvmti_env,
            PVOID jni_env,
            inspector::types::JavaClass* class_being_redefined,
            inspector::types::JavaObject* loader,
            const char* name,
            inspector::types::JavaObject* protection_domain,
            std::int32_t class_data_len,
            const std::uint8_t* class_data,
            std::int32_t* new_class_data_len,
            std::uint8_t** new_class_data
        );
    };
}