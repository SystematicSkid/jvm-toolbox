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

        /* Thread methods */
        virtual bool get_thread_state( void* thread, std::int32_t& state ) = 0;
        virtual bool get_current_thread( void*& thread ) = 0;
        virtual bool get_all_threads( std::vector<void*>& threads ) = 0;
        virtual bool suspend_thread( void* thread ) = 0;
        virtual bool resume_thread( void* thread ) = 0;
        virtual bool stop_thread( void* thread, void* exception ) = 0;
        virtual bool interrupt_thread( void* thread ) = 0;
        virtual bool get_thread_info( void* thread, std::string& name, std::int32_t& priority, bool& daemon, void*& thread_group, void*& context_class_loader ) = 0;

        /* Stack Frame methods */
        virtual bool get_stack_trace( void* thread, std::int32_t start_depth, std::int32_t max_frame_count, std::vector<void*>& frames ) = 0;
        virtual bool get_all_stack_traces( std::int32_t max_frame_count, std::vector<void*>& frames ) = 0;
        virtual bool get_frame_count( void* thread, std::int32_t& count ) = 0;
        virtual bool get_frame_location( void* thread, std::int32_t depth, void*& method, std::int32_t& location ) = 0;

        /* Object methods */
        virtual bool get_object_size( void* object, std::int32_t& size ) = 0;
        virtual bool get_object_hash_code( void* object, std::int32_t& hash_code ) = 0;

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

        /* Field Methods */
        virtual bool get_field_name( void* class_, void* field, std::string& name, std::string& signature, std::string& generic ) = 0;
        virtual bool get_field_declaring_class( void* class_, void* field, void*& declaring_class ) = 0;
        virtual bool get_field_modifiers( void* class_, void* field, std::int32_t& modifiers ) = 0;
        virtual bool get_field_offset( void* class_, void* field, std::int64_t& offset ) = 0;
        virtual bool get_field_is_synthetic( void* class_, void* field, bool& synthetic ) = 0;
    };
}