#include <iostream>
#include "headless.hpp"
#include "ipc/include/ipc.hpp"
#include "ipc/include/messages/inspector_info_generated.h"
#include "ipc/include/messages/inspector_data_generated.h"

void inspector::headless::on_message( const std::vector<std::uint8_t>& message )
{

}

void inspector::headless::setup_consumer( )
{
    printf( "Setting up consumer in headless mode...\n" );
    /* Setup consumer callback */
    std::unique_ptr<ipc::Consumer> consumer = std::make_unique<ipc::Consumer>(
        "inspector",
        consumer_size, 
        on_message 
    );

    printf( "Consumer setup.\n" );
}