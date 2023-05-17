#pragma once
#include <cstdint>

namespace inspector::types
{
    class JavaObject
    {
    private:
        void* _object;
    public:
        JavaObject( void* object );
        bool get_object_size( std::int32_t& size );
        bool get_object_hash_code( std::int32_t& hash_code );
    };
}