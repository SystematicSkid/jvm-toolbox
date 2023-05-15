#pragma once
#include <stdexcept>
#include <string>

namespace ipc
{
    class SharedMemoryException : public std::runtime_error
    {
    public:
        SharedMemoryException( const std::string& message ) 
            : std::runtime_error( message ) { };
    };
}