#pragma once
#include <string>
#include <vector>

namespace inspector::types
{
    class JavaClass
    {
    private:
        void* _class;
    public:
        bool get_class_signature( std::string& signature, std::string& generic_ptr );
        bool get_class_status( std::int16_t& status );
        bool get_class_source_file_name( std::string& source_file_name );
        bool get_class_modifiers( std::int32_t& modifiers );
        /* TODO: Implement java method and field class */
        bool get_class_methods( std::vector<void*>& methods );
        bool get_class_fields( std::vector<void*>& fields );
        bool get_class_interfaces( std::vector<void*>& interfaces );
        bool get_class_version( std::int32_t& minor, std::int32_t& major );
        /* TODO: Implement constant pool interfacing */
        bool get_class_constant_pool( std::vector<void*>& constant_pool );
        bool get_class_is_interface( bool& is_interface );
        bool get_class_is_array( bool& is_array );
        bool get_class_loader( void*& class_loader );
    };
}