#pragma once
#include <cstdint>
#include <string>

namespace inspector::types
{
    class JavaMethod
    {
    private:
        void* _method;
    public:
        JavaMethod( void* method ) : _method( method ) { }

        bool get_method_name( std::string& name );
        bool get_method_signature( std::string& signature );
        bool get_method_generic_signature( std::string& generic_signature );
        bool get_method_declaring_class( void*& class_ );
        bool get_method_modifiers( std::int32_t& modifiers );
        bool get_max_locals( std::int32_t& max_locals );
        bool get_arguments_size( std::int32_t& arguments_size );
        bool get_line_number_table( std::vector<std::pair<std::int32_t, std::int32_t>>& line_number_table );
        bool get_method_location( std::int32_t& start, std::int32_t& end );
        bool get_local_variable_table( std::vector<void*>& local_variable_table );
        bool get_bytecodes( std::vector<std::uint8_t>& bytecodes );
        bool get_method_is_native( bool& is_native );
        bool get_method_is_synthetic( bool& is_synthetic );
        bool get_method_is_obsolete( bool& is_obsolete );

        bool set_method_breakpoint( std::int64_t location );
        bool clear_method_breakpoint( std::int64_t location );
    };
}