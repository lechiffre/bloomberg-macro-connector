#include <blpconn.h>
#include <gtest/gtest.h>
#include <thread>
#include <chrono>

using namespace BlpConn;

TEST(Context, Subscription) {
    Context ctx;
    std::string config_path = "./config.json";
    EXPECT_TRUE(ctx.initializeSession(config_path));
    ctx.addNotificationHandler(defaultObserver);
    SubscriptionRequest request = {
            .topic = "CATBTOTB Index",
            .correlation_id = 12
    };
    EXPECT_EQ(ctx.subscribe(request), 0);
    std::this_thread::sleep_for(std::chrono::seconds(10));
    ctx.unsubscribe(request);
    ctx.shutdownSession();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
