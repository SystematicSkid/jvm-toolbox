#pragma once
#include <Windows.h>
#include "base_event.hpp"

namespace inspector::events
{
    class VMDeathEvent : public BaseEvent
    {
    private:
        inspector::interfaces::JavaInterface* _java_interface;
    public:
        bool setup( inspector::interfaces::JavaInterface* java_interface ) override;
        std::string get_name( ) override { return "VMDeathEvent"; }
        bool enable( ) override;
        bool disable( ) override;
    private:
        /* Our event */
        static void vm_death_hook(
            PVOID jvmti_env,
            PVOID jni_env
        );
    };
}