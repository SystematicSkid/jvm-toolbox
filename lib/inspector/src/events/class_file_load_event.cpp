#include "inspector.hpp"
#include "class_file_load_event.hpp"
#include <vector>
#include <memory>
#include <map>
#include <string>
#include "ipc/include/messages/inspector_data_generated.h"

bool inspector::events::ClassFileLoadEvent::setup( inspector::interfaces::JavaInterface* java_interface )
{
    _java_interface = java_interface;
    return true;
}

bool inspector::events::ClassFileLoadEvent::enable()
{
    /* Ensure we have an inspector interface setup */
    if ( !_java_interface )
        return false;
    /* Enable our event */
    if ( !_java_interface->set_class_file_load_event( class_file_load_hook ) )
    {
        printf( "Failed to enable class file load event.\n" );
        /* Get error message */
        auto error_message = _java_interface->get_last_error_message( );
        /* Print error message */
        printf( "Error: %s\n", error_message.c_str( ) );
        return false;
    }
    return true;
}

bool inspector::events::ClassFileLoadEvent::disable()
{
    /* Ensure we have an inspector interface setup */
    if ( !_java_interface )
        return false;
    
    /* Disable our event */
    _java_interface->set_class_file_load_event( nullptr );
    /* Handle error */
    if ( _java_interface->get_last_error( ) )
    {
        printf( "Failed to disable class file load event.\n" );
        /* Get error message */
        auto error_message = _java_interface->get_last_error_message( );
        /* Print error message */
        printf( "Error: %s\n", error_message.c_str( ) );
        return false;
    }

    return true;
}

void inspector::events::ClassFileLoadEvent::class_file_load_hook(
    PVOID jvmti_env, 
    PVOID jni_env, 
    inspector::types::JavaClass *class_being_redefined, 
    inspector::types::JavaObject *loader, 
    const char *name, 
    inspector::types::JavaObject *protection_domain, 
    std::int32_t class_data_len, 
    const std::uint8_t *class_data, 
    std::int32_t *new_class_data_len, 
    std::uint8_t **new_class_data)
{
    printf( "Class file load event.\n" );
    printf( "\tName: %s\n", name );
    /* Create our flatbuffer object */
    flatbuffers::FlatBufferBuilder builder;
    jvm_toolbox_flatbuffers::inspector::OnClassFileLoadBuilder packet( builder );

    packet.add_name( builder.CreateString( name ) );

    /* Finish our packet */
    builder.Finish( packet.Finish( ), "ClassFileLoadEvent" );

    /* Create our message */
    std::vector<unsigned char> message( builder.GetBufferPointer( ), builder.GetBufferPointer( ) + builder.GetSize( ) );
    printf("Builder size: %d\n", builder.GetSize());
    /* Produce our message */
    inspector::producer->produce( message );
}
