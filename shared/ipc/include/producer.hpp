#pragma once
#include "shared_memory_manager.hpp"

#include <atomic>
#include <thread>
#include <functional>
#include <memory>
#include <optional>
#include <condition_variable>
#include <iostream>

namespace ipc
{
    class Producer
    {
    public:
        Producer( const std::string& name, const std::size_t size )
            : _shared_memory_manager( std::make_shared<SharedMemoryManager>( name, size ) )
        { };

        void produce( const std::vector<unsigned char>& data )
        {
            std::unique_lock<std::mutex> lock( _mutex );
            /* Get size of data */
            auto size = data.size( );
            std::vector<unsigned char> buffer( sizeof( size ) + size );
            std::memcpy( buffer.data( ), &size, sizeof( size ) );
            std::memcpy( buffer.data( ) + sizeof( size ), data.data( ), size );
            /* Write data */
            this->_shared_memory_manager->write( 0, buffer.data( ), buffer.size( ) );
            _condition_variable.notify_one( );
        }
        
    private:
        std::shared_ptr<SharedMemoryManager> _shared_memory_manager;
        std::mutex _mutex;
        std::condition_variable _condition_variable;
    };
}