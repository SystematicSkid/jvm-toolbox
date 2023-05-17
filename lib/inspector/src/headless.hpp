#pragma once
#include <cstdint>
#include <vector>

namespace inspector::headless
{
    constexpr std::size_t consumer_size = 1024 * 1024;
    extern ipc::Consumer* consumer;
    void on_message( const std::vector<std::uint8_t>& message );
    void setup_consumer( );
}