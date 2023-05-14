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
        
        /* Thread methods */
        bool get_thread_state( void* thread, std::int32_t& state ) override;
        bool get_current_thread( void*& thread ) override;
        bool get_all_threads( std::vector<void*>& threads ) override;
        bool suspend_thread( void* thread ) override;
        bool resume_thread( void* thread ) override;
        bool stop_thread( void* thread, void* exception ) override;
        bool interrupt_thread( void* thread ) override;
        bool get_thread_info( void* thread, std::string& name, std::int32_t& priority, bool& daemon, void*& thread_group, void*& context_class_loader ) override;

        /* Stack Frame methods */
        bool get_stack_trace( void* thread, std::int32_t start_depth, std::int32_t max_frame_count, std::vector<void*>& frames ) override;
        bool get_all_stack_traces( std::int32_t max_frame_count, std::vector<void*>& frames ) override;
        bool get_frame_count( void* thread, std::int32_t& count ) override;
        bool get_frame_location( void* thread, std::int32_t depth, void*& method, std::int32_t& location ) override;

        /* Object methods */
        bool get_object_size( void* object, std::int32_t& size ) override;
        bool get_object_hash_code( void* object, std::int32_t& hash_code ) override;

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

        /* Field methods */
        bool get_field_name( void* class_, void* field, std::string& name, std::string& signature, std::string& generic ) override;
        bool get_field_declaring_class( void* class_, void* field, void*& declaring_class ) override;
        bool get_field_modifiers( void* class_, void* field, std::int32_t& modifiers ) override;
        bool get_field_offset( void* class_, void* field, std::int64_t& offset ) override;
        bool get_field_is_synthetic( void* class_, void* field, bool& synthetic ) override;
    };
}