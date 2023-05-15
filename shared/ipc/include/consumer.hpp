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
    class Consumer
    {
    public:
        using Callback = std::function<void(const std::string&)>;

        Consumer( const std::string& name, const std::size_t size, Callback callback )
            : _shared_memory_manager( std::make_shared<SharedMemoryManager>( name, size ) ), _callback( callback ),
            _stop( false )
        {
            this->_worker = std::thread( [this]() { this->run( ); } );
        };

        ~Consumer( )
        {
            this->_stop = true;
            this->_worker.join( );
        }

        template <typename T>
        std::optional<T> receive( )
        {
            std::unique_lock<std::mutex> lock( _mutex );
            std::chrono::milliseconds timeout( 100 );  
            if ( !_condition_variable.wait_for(lock, timeout, [this]() { return this->_shared_memory_manager->available( ); } ) ) {
                // Handle timeout
                return std::nullopt;
            }

            if( this->_shared_memory_manager->available( ) )
            {
                T data;
                this->_shared_memory_manager->read( 0, data );
                return data;
            }
            else
            {
                return std::nullopt;
            }
        }

        void run( )
        {
            while ( !_stop )
            {
                auto message = receive<std::string>( );
                if ( message.has_value( ) )
                {
                    _callback( message.value( ) );
                }
            }
        }

    private:
        std::shared_ptr<SharedMemoryManager> _shared_memory_manager;
        std::mutex _mutex;
        std::condition_variable _condition_variable;
        std::atomic<bool> _stop;
        std::thread _worker;
        Callback _callback;
    };
}