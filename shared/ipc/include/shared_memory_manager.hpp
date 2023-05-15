#pragma once
#include <string>
#include <Windows.h>
#include "shared_memory_exception.hpp"

namespace ipc
{
    class SharedMemoryManager
    {
    public:
        SharedMemoryManager( const std::string& name, const std::size_t size );
        ~SharedMemoryManager( );

        void write( const std::size_t offset, const void* data, const std::size_t size );
        void read( const std::size_t offset, void* data, const std::size_t size );
        bool available( );

        template<typename T>
        void write( const std::size_t offset, const T& data )
        {
            write( offset, &data, sizeof( T ) );
        }

        template<typename T>
        void read( const std::size_t offset, T& data )
        {
            read( offset, &data, sizeof( T ) );
        }

        template<typename T>
        T read( const std::size_t offset )
        {
            T data;
            read( offset, &data, sizeof( T ) );
            return data;
        }

        template<typename T>
        void write( const std::size_t offset, const T* data, const std::size_t size )
        {
            write( offset, static_cast<const void*>( data ), size );
        }

        template<typename T>
        void read( const std::size_t offset, T* data, const std::size_t size )
        {
            read( offset, static_cast<void*>( data ), size );
        }

        private:
            HANDLE _shared_memory_handle;
            HANDLE _mutex_handle;
            void* _shared_memory;

            std::string _name;
            std::size_t _size;

            void lock( );
            void unlock( );
    };
}