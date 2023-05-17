#include "inspector.hpp"
#include "vm_death_event.hpp"
#include <vector>
#include <memory>
#include <map>
#include <string>
#include "ipc/include/messages/inspector_data_generated.h"

bool inspector::events::VMDeathEvent::setup( inspector::interfaces::JavaInterface* java_interface )
{
    _java_interface = java_interface;
    return true;
}

bool inspector::events::VMDeathEvent::enable( )
{
    /* Ensure we have an inspector interface setup */
    if ( !_java_interface )
        return false;
    /* Enable our event */
    if ( !_java_interface->set_vm_death_event( vm_death_hook ) )
    {
        printf( "Failed to enable VM death event.\n" );
        /* Get error message */
        auto error_message = _java_interface->get_last_error_message( );
        /* Print error message */
        printf( "Error: %s\n", error_message.c_str( ) );
        return false;
    }
    return true;
}

bool inspector::events::VMDeathEvent::disable( )
{
    /* Ensure we have an inspector interface setup */
    if ( !_java_interface )
        return false;
    
    /* Disable our event */
    _java_interface->set_vm_death_event( nullptr );
    /* Handle error */
    if ( _java_interface->get_last_error( ) )
    {
        printf( "Failed to disable VM death event.\n" );
        /* Get error message */
        auto error_message = _java_interface->get_last_error_message( );
        /* Print error message */
        printf( "Error: %s\n", error_message.c_str( ) );
        return false;
    }

    return true;
}

void inspector::events::VMDeathEvent::vm_death_hook( PVOID jvmti_env, PVOID jni_env )
{
    /* Create our flatbuffer object */
    flatbuffers::FlatBufferBuilder builder;
    jvm_toolbox_flatbuffers::inspector::OnClassFileLoadBuilder packet( builder );

    /* Finish our packet */
    builder.Finish( packet.Finish( ), "VMDeathEvent" );

    /* Create our message */
    std::vector<unsigned char> message( builder.GetBufferPointer( ), builder.GetBufferPointer( ) + builder.GetSize( ) );
    /* Produce our message */
    inspector::producer->produce( message );
}
