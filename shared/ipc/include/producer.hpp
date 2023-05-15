#pragma once
#include "shared_memory_manager.hpp"

#include <atomic>
#include <thread>
#include <functional>
#include <memory>
#include <optional>
#include <condition_variable>

namespace ipc
{
    class Producer
    {
    public:
        Producer( const std::string& name, const std::size_t size )
            : _shared_memory_manager( std::make_shared<SharedMemoryManager>( name, size ) )
        { };

        template<typename T>
        void produce( const T& data )
        {
            std::unique_lock<std::mutex> lock( _mutex );
            _shared_memory_manager->write( 0, data );
            _condition_variable.notify_one( );
        }
        
    private:
        std::shared_ptr<SharedMemoryManager> _shared_memory_manager;
        std::mutex _mutex;
        std::condition_variable _condition_variable;
    };
}