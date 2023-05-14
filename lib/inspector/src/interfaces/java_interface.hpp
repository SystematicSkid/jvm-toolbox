#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace inspector::interfaces
{
    /* Abstract class to handle all of our calls to the jvm */
    class JavaInterface
    {
    public:
        /* Utility methods */
        virtual bool initialize( )  = 0;
        virtual bool destroy( )     = 0;
        virtual std::string get_name( ) = 0;
        virtual std::int32_t get_last_error( ) = 0;
        virtual std::string get_last_error_message( ) = 0;
        virtual void set_last_error( std::int32_t error ) = 0;

        /* Class methods */
        virtual bool get_loaded_classes( std::vector<void*>& classes ) = 0;
        virtual bool get_classloader_classes( void* classloader, std::vector<void*>& classes ) = 0;

        virtual bool get_class_signature( void* class_, std::string& signature, std::string& generic_ptr ) = 0;
    };
}