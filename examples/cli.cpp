#include <cstdio>
#include <readline/history.h>
#include <readline/readline.h>
#include <boost/algorithm/string.hpp>
#include <blpconn.h>
#include <blpconn_message.h>
#include <blpconn_fb_generated.h>

using namespace BlpConn;

enum {
    SUBSCRIBE,
    UNSUBSCRIBE
};

void observer(const uint8_t *buffer, size_t size) {
    flatbuffers::Verifier verifier(buffer, size);
    if (!BlpConn::FB::VerifyMessageVector(verifier, nullptr, nullptr)) {
        std::cout << "Invalid message" << std::endl;
        return;
    }
    auto main = flatbuffers::GetRoot<BlpConn::FB::Main>(buffer);
    if (main->message_type() == BlpConn::FB::Message_HeadlineEconomicEvent) {
        auto fb_event = main->message_as_HeadlineEconomicEvent();
        auto event = toHeadlineEconomicEvent(fb_event);
        std::cout << event << std::endl;
    } else if (main->message_type() == BlpConn::FB::Message_HeadlineCalendarEvent) {
        auto fb_event = main->message_as_HeadlineCalendarEvent();
        auto event = toHeadlineCalendarEvent(fb_event);
        std::cout << event << std::endl;
    } else if (main->message_type() == BlpConn::FB::Message_LogMessage) {
        auto fb_log_message = main->message_as_LogMessage();
        auto log_message = toLogMessage(fb_log_message);
        std::cout << log_message << std::endl;
    }
}

void eventRequest(Context& ctx, SubscriptionType subscription_type, const std::string& rem, int action) {
    SubscriptionRequest request = {
        .topic = rem,
        .subscription_type = subscription_type
    };
    if (action == SUBSCRIBE) {
        ctx.subscribe(request);
    } else {
        ctx.unsubscribe(request);
    }
}

void processCommand(Context& ctx, const std::string& line) {
    size_t pos = line.find(' ');
    if (pos == line.npos) {
        std::cout << "Invalidad command" << std::endl;
        return;
    }
    std::string cmd = line.substr(0, pos);
    std::string rem = line.substr(pos);
    boost::trim(rem);
    pos = rem.find(' ');
    if (pos == line.npos) {
        std::cout << "Invalidad event" << std::endl;
        return;
    }
    std::string evt = rem.substr(0, pos);
    std::string par = rem.substr(pos);
    boost::algorithm::trim(cmd);
    boost::algorithm::trim(evt);
    boost::algorithm::trim(par);
    std::cout << cmd << ":" << evt << ":" << par << std::endl;
    if (cmd == "subscribe") {
        if (evt == "calendar") {
            eventRequest(ctx, SubscriptionType::ReleaseCalendar, par, SUBSCRIBE); 
        } else if (evt == "economic") {
            eventRequest(ctx, SubscriptionType::HeadlineActuals, par, SUBSCRIBE); 
        } else {
            std::cout << "Not defined command" << std::endl;
        }
    } else if (cmd == "unsubscribe") {
        if (evt == "calendar") {
            eventRequest(ctx, SubscriptionType::ReleaseCalendar, par, UNSUBSCRIBE); 
        } else if (evt == "economic") {
            eventRequest(ctx, SubscriptionType::HeadlineActuals, par, UNSUBSCRIBE); 
        } else {
            std::cout << "Not defined command" << std::endl;
        }
    }
    return;
}

void run(Context& ctx) {
    char *input;
    // Setting line history parameters
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
            // Clear the current input line
            processCommand(ctx, line);
        } catch (int e) {
            // Clear the current input line
            std::cout << "Error processing line" << std::endl;
        }
        // Redisplay the prompt after output
        std::cout << "> " << std::flush;
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
