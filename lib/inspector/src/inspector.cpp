#include <Windows.h>
#include "inspector.hpp"
#include "headless.hpp"
#include "interfaces/jvmti_interface.hpp"

/* Define extern symbols */
std::unique_ptr<inspector::interfaces::JavaInterface> inspector::java_interface;
HANDLE inspector::inspector_module;
std::unique_ptr<ipc::Producer> inspector::producer;

void setup_ipc( )
{
    /* Create producer with 1mb size */
    inspector::producer = std::make_unique<ipc::Producer>( "inspector", 1024 * 1024 );
    flatbuffers::FlatBufferBuilder builder;
    jvm_toolbox_flatbuffers::inspector::OnInspectorLoadedBuilder packet( builder );

    packet.add_inspector_interface( builder.CreateString( inspector::java_interface->get_name( ) ) );

    builder.Finish( packet.Finish( ) );

    std::vector<unsigned char> message( builder.GetBufferPointer( ), builder.GetBufferPointer( ) + builder.GetSize( ) );
    inspector::producer->produce(message);
}

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
    printf( "Initializing IPC...\n" );
    setup_ipc( );

    std::this_thread::sleep_for( std::chrono::milliseconds( 100 ) );

    #ifndef HEADLESS_MODE
    inspector::headless::setup_consumer( );
    #endif

    inspector::events::EventManager* event_manager = inspector::events::EventManager::get( );
    if(!event_manager->setup_events( inspector::java_interface.get( ) ))
    {
        printf( "Failed to setup events.\n" );
        return;
    }
    if(!event_manager->enable_all_events( ))
    {
        printf( "Failed to enable events.\n" );
        return;
    }

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

            std::string signature;
            method->get_method_signature( signature );
            if( inspector::java_interface->get_last_error( ) )
            {
                printf( "Failed to get method signature: %s\n", inspector::java_interface->get_last_error_message( ).c_str( ) );
                continue;
            }

            printf( "\t\tSignature: %s\n", signature.c_str( ) );
            
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