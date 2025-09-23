#include <blpconn.h>
#include <gtest/gtest.h>
#include <thread>
#include <chrono>

using namespace BlpConn;

TEST(Connection, ConnectDisconnect) {
    Context ctx;
    std::string config_path = "./config.json";
    EXPECT_TRUE(ctx.initializeSession(config_path));
    std::this_thread::sleep_for(std::chrono::seconds(3));
    ctx.shutdownSession();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
