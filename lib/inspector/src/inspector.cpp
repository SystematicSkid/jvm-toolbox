#include <Windows.h>
#include "inspector.hpp"
#include "interfaces/jvmti_interface.hpp"

void initialize( )
{
    /* AllocConsole */
    AllocConsole( );
    freopen_s( ( FILE** )stdout, "CONOUT$", "w", stdout );

    /* Initialize inspector */
    inspector::java_interface = std::make_unique<inspector::interfaces::JvmtiInterface>( );
    std::vector<void*> classes;
    inspector::java_interface->get_loaded_classes( classes );

    printf( "Loaded %zd classes\n", classes.size( ) );

    while( true )
    {
        if( GetAsyncKeyState( VK_END ) )
            break;
    }

    /* Free Console */
    fclose( ( FILE* )stdout );
    FreeConsole( );

}

BOOL APIENTRY DllMain( HMODULE dll_module, DWORD reason, LPVOID reserved )
{
    if( reason != DLL_PROCESS_ATTACH )
        return TRUE;
    inspector::inspector_module = dll_module;

    /* CreateThread */
    CreateThread( nullptr, 0, ( LPTHREAD_START_ROUTINE )initialize, nullptr, 0, nullptr );

    return TRUE;
}