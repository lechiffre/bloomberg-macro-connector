#include <chrono>
#include <thread>
#include <blpconn.h>

using namespace BlpConn;

int main() {
    Context ctx;
    std::string config_path = "./config.json";
    ctx.initializeService(config_path);
    SubscriptionRequest request1 = {.topic = "CATBTOTB Index"};
    ctx.subscribe(request1);
    SubscriptionRequest request2 = {
        .topic = "INJCJC Index",
        .subscription_type = SubscriptionType::ReleaseCalendar,
    };
    ctx.subscribe(request2);
    std::this_thread::sleep_for(std::chrono::seconds(10));
    ctx.shutdownService();
}
