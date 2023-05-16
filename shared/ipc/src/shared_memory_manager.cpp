#include "../include/shared_memory_manager.hpp"
#include <mutex>
#include <iostream>

ipc::SharedMemoryManager::SharedMemoryManager( const std::string& name, const std::size_t size )
    : _name( name ), _size( size ), _shared_memory_handle( nullptr ), _mutex_handle( nullptr ), _shared_memory( nullptr )
{
    _shared_memory_handle = CreateFileMappingA(
        INVALID_HANDLE_VALUE, 
        nullptr, 
        PAGE_READWRITE, 
        0, 
        size, 
        name.c_str( ) 
    );

    if ( !_shared_memory_handle )
    {
        throw ipc::SharedMemoryException( "Failed to create shared memory" );
    }

    _shared_memory = MapViewOfFile( 
        _shared_memory_handle, 
        FILE_MAP_ALL_ACCESS, 
        0, 
        0, 
        size 
    );

    if ( !_shared_memory )
    {
        CloseHandle( _shared_memory_handle );
        throw ipc::SharedMemoryException( "Failed to map view of file" );
    }

    _mutex_handle = CreateMutexA( 
        nullptr, 
        FALSE, 
        ( name + "_ipc_mutex" ).c_str( ) 
    );

    if ( !_mutex_handle )
    {
        UnmapViewOfFile( _shared_memory );
        CloseHandle( _shared_memory_handle );
        throw ipc::SharedMemoryException( "Failed to create mutex" );
    }
}

ipc::SharedMemoryManager::~SharedMemoryManager( )
{
    if( _shared_memory )
    {
        UnmapViewOfFile( _shared_memory );
    }
    
    if( _shared_memory_handle )
    {
        CloseHandle( _shared_memory_handle );
    }

    if( _mutex_handle )
    {
        CloseHandle( _mutex_handle );
    }
}

void ipc::SharedMemoryManager::lock( )
{
    DWORD result = WaitForSingleObject( _mutex_handle, INFINITE );
    if( result != WAIT_OBJECT_0 )
    {
        throw ipc::SharedMemoryException( "Failed to lock mutex" );
    }
}

void ipc::SharedMemoryManager::unlock( )
{
    if( !ReleaseMutex( _mutex_handle ) )
    {
        throw ipc::SharedMemoryException( "Failed to unlock mutex" );
    }
}

void ipc::SharedMemoryManager::write( const std::size_t offset, const void* data, const std::size_t size )
{
    std::lock_guard<std::mutex> lock_( _mutex );
    std::memcpy( static_cast<char*>( _shared_memory ) + offset, data, size );
}

void ipc::SharedMemoryManager::read( const std::size_t offset, void* data, const std::size_t size )
{
    std::lock_guard<std::mutex> lock_( _mutex );
    std::memcpy( data, static_cast<char*>( _shared_memory ) + offset, size );
}

std::size_t ipc::SharedMemoryManager::size( ) const
{
    return _size;
}

std::size_t ipc::SharedMemoryManager::get_message_size( )
{
    std::lock_guard<std::mutex> lock_( _mutex );
    /* Read first 8 bytes */
    std::size_t message_size = 0;
    std::memcpy( &message_size, _shared_memory, sizeof( std::size_t ) );
    return message_size;
}

bool ipc::SharedMemoryManager::available( )
{
    std::lock_guard<std::mutex> lock_( _mutex );
    for ( std::size_t i = 0; i < sizeof( std::size_t ); ++i )
    {
        if ( static_cast<char*>(_shared_memory)[i] != 0 )
        {
            return true;
        }
    }
    return false;
}