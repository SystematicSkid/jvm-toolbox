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
    class Consumer
    {
    public:
        using Callback = std::function<void(const std::vector<unsigned char>&)>;

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

        
        std::optional<std::vector<unsigned char>> receive( )
        {
            std::unique_lock<std::mutex> lock( _mutex );
             std::chrono::nanoseconds timeout( 1 );  
            if ( !_condition_variable.wait_for(lock, timeout, [this]() { return this->_shared_memory_manager->available( ); } ) ) {
                // Handle timeout
                return std::nullopt;
            }

            /* Read size */
            auto size = this->_shared_memory_manager->get_message_size( );

            std::vector<unsigned char> buffer( size + sizeof( size ) );
            /* Read data */
            this->_shared_memory_manager->read( 0, buffer.data( ), buffer.size( ) );
            // Extract the size and the data from the buffer
            //std::memcpy( &size, buffer.data( ), sizeof( size ) );
            std::vector<unsigned char> data( buffer.begin( ) + sizeof( size ), buffer.end( ) );
            // Consume the data by clearing the shared memory
            //std::fill( buffer.begin( ), buffer.end( ), 0 );
            this->_shared_memory_manager->pop( );
            return data;

        }

        void run( )
        {
            while ( !_stop )
            {
                auto message = receive( );
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