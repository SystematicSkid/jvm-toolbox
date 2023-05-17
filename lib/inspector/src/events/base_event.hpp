#pragma once

namespace inspector::events
{
    class BaseEvent
    {
    public:
        virtual bool handle( ) = 0;
    }
}