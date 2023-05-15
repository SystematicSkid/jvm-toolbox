#include <gtest/gtest.h>
#include "../include/shared_memory_manager.hpp"
#include <cstring>
#include <array>

class SharedMemoryManagerTest : public ::testing::Test
{
protected:
    ipc::SharedMemoryManager* shared_memory_manager;
    std::string name;
    std::size_t size;

    void SetUp( ) override
    {
        name = "test_shared_memory";
        size = 256;
        shared_memory_manager = new ipc::SharedMemoryManager( name, size );
    }

    void TearDown( ) override
    {
        delete shared_memory_manager;
    }
};

TEST_F( SharedMemoryManagerTest, WriteAndRead )
{
    const char* write_data = "Hello World!";
    shared_memory_manager->write( 0, write_data, std::strlen( write_data ) + 1 );

    char read_data[256];
    shared_memory_manager->read( 0, read_data, 256 );

    EXPECT_STREQ( write_data, read_data );
}

TEST_F(SharedMemoryManagerTest, WriteAndReadPrimitive) {
    int writeData = 12345;
    shared_memory_manager->write(0, writeData);

    int readData;
    shared_memory_manager->read(0, readData);

    EXPECT_EQ(writeData, readData);
}

TEST_F(SharedMemoryManagerTest, WriteAndReadArray) {
    std::array<int, 4> writeData = {1, 2, 3, 4};
    shared_memory_manager->write(0, writeData.data(), writeData.size() * sizeof(int));

    std::array<int, 4> readData;
    shared_memory_manager->read(0, readData.data(), readData.size() * sizeof(int));

    EXPECT_EQ(writeData, readData);
}

TEST_F(SharedMemoryManagerTest, WriteAndReadString) {
    std::string writeData = "Hello, shared memory!";
    shared_memory_manager->write(0, writeData.c_str(), writeData.size() + 1);

    char readData[256];
    shared_memory_manager->read(0, readData, 256);

    EXPECT_STREQ(writeData.c_str(), readData);
}
