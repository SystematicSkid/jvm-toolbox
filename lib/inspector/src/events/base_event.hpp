#pragma once
#include <string>
#include <memory>
#include "../interfaces/java_interface.hpp"

namespace inspector::events
{
    class BaseEvent
    {
    public:
        virtual bool setup( std::unique_ptr<inspector::interfaces::JavaInterface> java_interface ) = 0;
        virtual std::string get_name( ) = 0;
        virtual bool enable( ) = 0;
        virtual bool disable( ) = 0;
    };
}