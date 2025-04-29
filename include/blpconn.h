#ifndef _BLPCONN_H
#define _BLPCONN_H

#include <string>
#include <vector>
#include <iostream>
#include <nlohmann/json.hpp>
#include <blpapi_session.h>

using json = nlohmann::json;
using namespace BloombergLP;

typedef void (*observerFunc)(const char* json_);

namespace BlpConn {

enum class EventType {
    HeadLineActuals,
    ReleaseCalendar,
    HeadLineSurveys
};

enum class TopicType {
    Ticker,
    Cusip,
};

struct SubscriptionRequest {
    std::string topic;
    TopicType topic_type = TopicType::Ticker;
    EventType event_type = EventType::HeadLineActuals;
    std::string options = "";
    std::string toUri();
};

class Logger {
public:
    Logger(std::ostream* out_stream = &std::cout): out_stream_(out_stream) {}
    void addNotificationHandler(observerFunc fnc) noexcept;
    void log(const std::string& json_message);
private:
    std::ostream* out_stream_;
    std::vector<observerFunc> callbacks_;
};

class EventHandler: public blpapi::EventHandler {
    public:
        void setLogger(Logger *prt_logger) { prt_logger_ = prt_logger; }
        bool processEvent(const blpapi::Event& event, blpapi::Session *session) override;
    private:
        Logger *prt_logger_;
};

class Context {
public:
    Context() {
        event_handler_.setLogger(&logger_);
    }

    /**
     * Authentication and connection to Bloomberg service
     */
    bool initializeService(std::string& config_path);
    void shutdownService();
    bool isConnected() { return session_ != nullptr; }
    int subscribe(SubscriptionRequest& request);
    void addNotificationHandler(observerFunc fnc) noexcept;
    void log(const std::string &json_message) {
        logger_.log(json_message);
    }
private:
    std::string service_ = "//blp/economic-data";
    Logger logger_;
    EventHandler event_handler_;
    blpapi::Session *session_ = nullptr;
    int subscription_counter_ = 0;
};

}

json newLogMessage(const std::string& module_name);


void parseElement(const blpapi::Element& elem, json j);

#endif // _BLPCONN_H
