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
    };
}