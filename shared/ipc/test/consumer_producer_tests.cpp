#include <gtest/gtest.h>
#include "../include/producer.hpp"
#include "../include/consumer.hpp"
#include "./flatbuffers/test_generated.h"
#include <string>
#include <thread>

class IPCManagerTest : public ::testing::Test
{
protected:
    static constexpr std::size_t size = 256;
    std::string name;

    void SetUp() override
    {
        name = "test_ipc";
    }
};

TEST_F(IPCManagerTest, WriteAndReadString)
{
    ipc::Producer producer(name, size);
    ipc::Consumer consumer(name, size,
                           [](const std::vector<unsigned char> &message)
                           {
                               /* Cast message to string */
                               //std::string message_string(message.begin(), message.end());
                               EXPECT_EQ((char*)message.data(), "A really super long string with many characters and many words lolololol!");
                           });

    std::string message = "A really super long string with many characters and many words lolololol!";
    std::vector<unsigned char> message_vector(message.begin(), message.end());
    producer.produce(message_vector);
}

TEST_F(IPCManagerTest, WriteAndReadMultipleStrings)
{
    std::vector<std::string> messages = {"Hello, IPC!", "Another message", "And another one"};
    std::vector<std::string> received;

    ipc::Producer producer(name, size);
    ipc::Consumer consumer(name, size, [&](const std::vector<unsigned char> &message)
                           {
                                /* Cast message to string */
                                received.emplace_back(message.begin(), message.end()); 
                            });

    for (const auto &message : messages)
    {
        std::vector<unsigned char> message_vector(message.begin(), message.end());
        producer.produce(message_vector);
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(5)); // Give the consumer time to process the message.
    EXPECT_EQ(messages, received);
}

TEST_F(IPCManagerTest, WriteAndReadLargeMessage)
{
    std::vector<unsigned char> message(1024 * 1024 * 1024 - 16, '1');
    std::vector<unsigned char> received;

    ipc::Producer producer(name, 1024 * 1024 * 1024);
    ipc::Consumer consumer(name, 1024 * 1024 * 1024, [&](const std::vector<unsigned char> &message)
                           {
                                received = message;
                                EXPECT_EQ(message, received);
                            });

    producer.produce(message);
}

TEST_F(IPCManagerTest, WriteAndReadFlatbuffer)
{
    flatbuffers::FlatBufferBuilder builder;
    jvm_toolbox::test::basic_tableBuilder basic_table_builder(builder);

    basic_table_builder.add_a(1);
    basic_table_builder.add_b(2);
    basic_table_builder.add_c(3);

    auto basic_table = basic_table_builder.Finish( );

    builder.Finish(basic_table);

    ipc::Producer producer(name, size);
    ipc::Consumer consumer(name, size,
                           [&](const std::vector<unsigned char> &message)
                           {
                               auto basic_table = flatbuffers::GetRoot<jvm_toolbox::test::basic_table>( message.data( ) );
                               EXPECT_EQ(basic_table->a(), 1);
                               EXPECT_EQ(basic_table->b(), 2);
                               EXPECT_EQ(basic_table->c(), 3);
                           });
    
    std::vector<unsigned char> message( builder.GetBufferPointer( ), builder.GetBufferPointer( ) + builder.GetSize( ) );
    producer.produce(message);
}