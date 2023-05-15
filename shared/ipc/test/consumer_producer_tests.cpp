#include <gtest/gtest.h>
#include "../include/producer.hpp"
#include "../include/consumer.hpp"
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
    ipc::Consumer consumer(name, size, [](const std::string &message)
                           { EXPECT_EQ(message, "Hello, IPC!"); });

    std::this_thread::sleep_for(std::chrono::seconds(1)); // Give the consumer time to start up.

    producer.produce<std::string>("Hello, IPC!");

    std::this_thread::sleep_for(std::chrono::seconds(1)); // Give the consumer time to process the message.
}

TEST_F(IPCManagerTest, WriteAndReadMultipleStrings)
{
    std::vector<std::string> messages = {"Hello, IPC!", "Another message", "And another one"};
    std::vector<std::string> received;

    ipc::Producer producer(name, size);
    ipc::Consumer consumer(name, size, [&](const std::string &message)
                           { received.push_back(message); });

    std::this_thread::sleep_for(std::chrono::seconds(1)); // Give the consumer time to start up.

    for (const auto &message : messages)
    {
        producer.produce<std::string>(message);
        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // Give the consumer time to process each message.
    }

    EXPECT_EQ(messages, received);
}
