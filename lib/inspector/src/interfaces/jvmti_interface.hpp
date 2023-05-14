#pragma once
#include <cstdint>
#include <string>

#include "java_interface.hpp"

/* jvmti */
#include <jni.h>
#include <jvmti.h>

namespace inspector::interfaces
{
    class JvmtiInterface : public JavaInterface
    {
    private:
        jvmtiEnv* _jvmti_env;
        std::int32_t _last_error;
        std::string _last_error_message;
    public:
        JvmtiInterface( JavaVM* jvm );
        ~JvmtiInterface( );

        /* Utility methods */
        bool initialize( ) override;
        bool destroy( ) override;
        std::string get_name( ) override;
        std::int32_t get_last_error( ) override;
        std::string get_last_error_message( ) override;
        void set_last_error( std::int32_t error ) override;

        /* Class methods */
        bool get_loaded_classes( std::vector<void*>& classes ) override;
        bool get_classloader_classes( void* classloader, std::vector<void*>& classes ) override;

        bool get_class_signature( void* class_, std::string& signature, std::string& generic_ptr ) override;
        bool get_class_status( void* class_, std::int16_t& status ) override;
        bool get_class_source_file_name( void* class_, std::string& source_file_name ) override;
        bool get_class_modifiers( void* class_, std::int32_t& modifiers ) override;
        bool get_class_methods( void* class_, std::vector<void*>& methods ) override;
        bool get_class_fields( void* class_, std::vector<void*>& fields ) override;
        bool get_class_interfaces( void* class_, std::vector<void*>& interfaces ) override;
        bool get_class_version( void* class_, std::int32_t& minor, std::int32_t& major ) override;
        bool get_class_constant_pool( void* class_, std::vector<unsigned char>& constant_pool ) override;
        bool get_class_is_interface( void* class_, bool& is_interface ) override;
        bool get_class_is_array( void* class_, bool& is_array ) override;
        bool get_class_loader( void* class_, void*& class_loader ) override;
    };
}