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
        virtual bool get_class_status( void* class_, std::int16_t& status ) = 0;
        virtual bool get_class_source_file_name( void* class_, std::string& source_file_name ) = 0;
        virtual bool get_class_modifiers( void* class_, std::int32_t& modifiers ) = 0;
        virtual bool get_class_methods( void* class_, std::vector<void*>& methods ) = 0;
        virtual bool get_class_fields( void* class_, std::vector<void*>& fields ) = 0;
        virtual bool get_class_interfaces( void* class_, std::vector<void*>& interfaces ) = 0;
        virtual bool get_class_version( void* class_, std::int32_t& minor, std::int32_t& major ) = 0;
        virtual bool get_class_constant_pool( void* class_, std::vector<unsigned char>& constant_pool ) = 0;
        virtual bool get_class_is_interface( void* class_, bool& is_interface ) = 0;
        virtual bool get_class_is_array( void* class_, bool& is_array ) = 0;
        virtual bool get_class_loader( void* class_, void*& class_loader ) = 0;
    };
}