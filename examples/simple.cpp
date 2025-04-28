#include <chrono>
#include <thread>
#include <blpconn.h>

using namespace BlpConn;

int main() {
    Context ctx;
    std::string config_path = "./config.json";
    ctx.initializeService(config_path);
    SubscriptionRequest request = {.topic = "CATBTOTB Index"};
    ctx.subscribe(request);
    std::this_thread::sleep_for(std::chrono::seconds(5));
    ctx.shutdownService();
}
