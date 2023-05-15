#include "../include/shared_memory_manager.hpp"

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

    printf("Shared memory manager created\n");
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
    lock( );
    std::memcpy( static_cast<char*>( _shared_memory ) + offset, data, size );
    unlock( );
}

void ipc::SharedMemoryManager::read( const std::size_t offset, void* data, const std::size_t size )
{
    lock( );
    std::memcpy( data, static_cast<char*>( _shared_memory ) + offset, size );
    unlock( );
}

bool ipc::SharedMemoryManager::available( )
{
    lock( );
    bool result = std::memcmp( _shared_memory, "\0", 1 ) != 0;
    unlock( );
    return result;
}