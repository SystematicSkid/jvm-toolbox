#include "event_manager.hpp"

inspector::events::EventManager::EventManager()
{
    /* Add events */
    this->add_event( std::make_unique<inspector::events::ClassFileLoadEvent>( ) );
    this->add_event( std::make_unique<inspector::events::VMDeathEvent>( ) );
}

bool inspector::events::EventManager::add_event(std::unique_ptr<inspector::events::BaseEvent> event)
{
    if ( !event )
        return false;
    _events.insert( std::make_pair( event->get_name( ), std::move( event ) ) );
    return true;
}

bool inspector::events::EventManager::setup_events(inspector::interfaces::JavaInterface *java_interface)
{
    for ( auto& event : _events )
    {
        if ( !event.second->setup( java_interface ) )
            return false;
    }
    return true;
}

bool inspector::events::EventManager::enable_all_events()
{
    for( auto& event : _events )
        event.second->enable( );
    return true;
}

bool inspector::events::EventManager::disable_all_events( )
{
    for( auto& event : _events )
        event.second->disable( );
    return true;
}

bool inspector::events::EventManager::enable_event( std::string& name )
{
    auto event = this->get_event<inspector::events::BaseEvent>( name );
    if ( !event )
        return false;
    return event->enable( );
}

bool inspector::events::EventManager::disable_event( std::string& name )
{
    auto event = this->get_event<inspector::events::BaseEvent>( name );
    if ( !event )
        return false;
    return event->disable( );
}