#include <iostream>
#include "inspector.hpp"
#include "headless.hpp"
#include "ipc/include/ipc.hpp"
#include "ipc/include/messages/inspector_info_generated.h"
#include "ipc/include/messages/inspector_data_generated.h"

ipc::Consumer* inspector::headless::consumer = nullptr;

void inspector::headless::on_message( const std::vector<std::uint8_t>& message )
{
    if(flatbuffers::BufferHasIdentifier( message.data( ), "ClassFileLoadEvent" ))
    {
        auto event = flatbuffers::GetRoot<jvm_toolbox_flatbuffers::inspector::OnClassFileLoad>( message.data( ) );
        std::cout << "ClassFileLoadEvent: " << event->name()->c_str() << std::endl;
        std::vector<std::uint8_t> bytecode = { event->bytecode()->begin(), event->bytecode()->end() };
        std::cout << "Bytecode: ";
        for( auto& byte : bytecode )
        {
            std::cout << std::hex << (int)byte << " ";
        }
        std::cout << std::endl;
    }
    else
    {
        std::cout << "Unknown event received." << std::endl;
    }
}

void inspector::headless::setup_consumer( )
{
    printf( "Setting up consumer in headless mode...\n" );
    /* Setup consumer callback */
    headless::consumer = new ipc::Consumer("inspector", 1024 * 1024,
        [](const std::vector<unsigned char>& message)
        {
            inspector::headless::on_message( message );
        });

    printf( "Consumer setup.\n" );
}