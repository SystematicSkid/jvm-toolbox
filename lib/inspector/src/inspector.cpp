#include <Windows.h>
#include "inspector.hpp"
#include "interfaces/jvmti_interface.hpp"

/* Define extern symbols */
std::unique_ptr<inspector::interfaces::JavaInterface> inspector::java_interface;
HANDLE inspector::inspector_module;

void initialize( )
{
    /* AllocConsole */
    AllocConsole( );
    freopen_s( ( FILE** )stdout, "CONOUT$", "w", stdout );

    printf( "Initializing inspector...\n");

    /* Initialize inspector */
    inspector::java_interface = std::make_unique<inspector::interfaces::JvmtiInterface>( );
    /* Check for error */
    if( inspector::java_interface->get_last_error( ) != 0 )
    {
        printf( "Failed to initialize inspector: %s\n", inspector::java_interface->get_last_error_message( ).c_str( ) );
    }
    printf( "Inspector initialized\n" );
    std::vector<std::unique_ptr<inspector::types::JavaClass>> classes;
    inspector::java_interface->get_loaded_classes( classes );

    printf( "Loaded %zd classes\n", classes.size( ) );

    for( auto& class_ : classes )
    {
        std::string file_name;
        class_->get_class_source_file_name( file_name );
        if(inspector::java_interface->get_last_error())
            continue;

        std::vector<std::unique_ptr<inspector::types::JavaMethod>> methods;
        class_->get_class_methods( methods );

        printf( "Class: %s\n", file_name.c_str( ) );
        
        for( auto& method : methods )
        {
            std::string name;
            method->get_method_name( name );
            if( inspector::java_interface->get_last_error( ) )
            {
                printf( "Failed to get method name: %s\n", inspector::java_interface->get_last_error_message( ).c_str( ) );
                continue;
            }
            printf( "\tMethod: %s\n", name.c_str( ) );
        }
    }

    while( true )
    {
        if( GetAsyncKeyState( VK_END ) )
            break;
    }

    /* Free Console */
    fclose( ( FILE* )stdout );
    FreeConsole( );

    /* Free Library */
    FreeLibraryAndExitThread( reinterpret_cast<HMODULE>( inspector::inspector_module ), 0 );
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