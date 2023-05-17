#pragma once
#include <cstdint>
#include <unordered_map>

#include "base_event.hpp"
#include "class_file_load_event.hpp"

namespace inspector::events
{
    class EventManager
    {
    private:
        std::unordered_map<std::string, std::unique_ptr<BaseEvent>> _events;
    public:
        /* Singleton */
        static EventManager* get( )
        {
            static EventManager* instance = new EventManager( );
            return instance;
        }

        EventManager( );

        bool add_event( std::unique_ptr<BaseEvent> event );

        bool enable_all_events( );
        bool disable_all_events( );

        bool enable_event( std::string& name );
        bool disable_event( std::string& name );

        template <typename T>
        T* get_event( std::string& name )
        {
            auto event = _events.find( name );
            if ( event == _events.end( ) )
                return nullptr;
            return dynamic_cast<T*>( event->second.get( ) );
        }

        template <typename T>
        T* get_event( )
        {
            for ( auto& event : _events )
            {
                auto casted_event = dynamic_cast<T*>( event.second.get( ) );
                if ( casted_event )
                    return casted_event;
            }
            return nullptr;
        }
    };
}