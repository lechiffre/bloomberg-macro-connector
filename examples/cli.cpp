#include <cstdio>
#include <readline/history.h>
#include <readline/readline.h>
#include <boost/algorithm/string.hpp>
#include <blpconn.h>
#include <blpconn_fb_generated.h>

using namespace BlpConn;


void observer(const uint8_t *buffer, size_t size) {
    std::cout << ">>>> CLI observer" << std::endl;
    flatbuffers::Verifier verifier(buffer, size);
    if (!BlpConn::FB::VerifyMessageVector(verifier, nullptr, nullptr)) {
        std::cout << "Invalid message" << std::endl;
        return;
    }
    auto main = flatbuffers::GetRoot<BlpConn::FB::Main>(buffer);
    if (main->message_type() == BlpConn::FB::Message_HeadlineEconomicEvent) {
        auto event = main->message_as_HeadlineEconomicEvent();
        std::cout << "Received economic event: " << event->description()->str() << std::endl;
    } else if (main->message_type() == BlpConn::FB::Message_HeadlineCalendarEvent) {
        auto event = main->message_as_HeadlineCalendarEvent();
        std::cout << "Received calendar event: " << event->description()->str() << std::endl;
    } else if (main->message_type() == BlpConn::FB::Message_LogMessage) {
        auto log_message = main->message_as_LogMessage();
        std::cout << "Log message: " << log_message->message()->str() << std::endl;
    } else {
        std::cout << "Unknown message type" << std::endl;
    }
}


void processEventRequest(Context& ctx, SubscriptionType subscription_type, const std::string& rem) {
    SubscriptionRequest request = {
        .topic = rem,
        .subscription_type = subscription_type
    };
    ctx.subscribe(request);
}

void processCalendarEvent(Context& ctx, const std::string& rem) {
    processEventRequest(ctx, SubscriptionType::ReleaseCalendar, rem); 
}

void processEconomicEvent(Context& ctx, const std::string& rem) {
    processEventRequest(ctx, SubscriptionType::HeadLineActuals, rem);
}

void processCommand(Context& ctx, const std::string& line) {
    size_t pos = line.find(' ');
    if (pos == line.npos) {
        std::cout << "Invalidad command" << std::endl;
        return;
    }
    std::string cmd = line.substr(0, pos);
    std::string rem = line.substr(pos);
    boost::algorithm::trim(cmd);
    boost::algorithm::trim(rem);
    if (cmd == "calendar") {
        processCalendarEvent(ctx, rem);
    } else if (cmd == "economic") {
        processEconomicEvent(ctx, rem);
    } else {
        std::cout << "Not defined command" << std::endl;
    }
    return;
}

void run(Context& ctx) {
    char *input;
    // Seting line history parameters
    std::string history_path =
        std::string(std::getenv("HOME")) + "/.blpconn_history";
    using_history();
    read_history(history_path.c_str());
    stifle_history(1000);
    // Command loop
    while (true) {
        input = readline("> ");
        if (!input) {
            break;
        }
        std::string line(input);
        if (line.length() > 0) {
            add_history(input);
        }
        free(input);
        if (line == "quit") {
            break;
        }
        try {
            processCommand(ctx, line);
        } catch (int e) {
            std::cout << "Error processing line" << std::endl;
        }
    }
    write_history(history_path.c_str());
}

int main() {
    Context ctx;
    ctx.addNotificationHandler(observer);
    std::string config_path = "./config.json";
    ctx.initializeService(config_path);
    run(ctx);      
}
