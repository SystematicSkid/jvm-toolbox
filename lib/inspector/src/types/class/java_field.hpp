#pragma once
#include <cstdint>
#include <string>

namespace inspector::types
{
    class JavaField
    {
    private:
        void* _class;
        void* _field;
    public:
        bool get_field_name( std::string& name );
        bool get_field_signature( std::string& signature );
        bool get_field_generic_signature( std::string& generic_signature );
        bool get_field_declaring_class( void*& class_ );
        bool get_field_modifiers( std::int32_t& modifiers );
        bool get_field_offset( std::int64_t& offset );
        bool get_is_synthetic( bool& synthetic );
    };
}