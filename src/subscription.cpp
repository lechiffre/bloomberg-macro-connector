#include "blpconn.h"

static const char *module_name = "SubscriptionService";

namespace BlpConn {

std::string SubscriptionRequest::toUri() {
    std::string uri = "";
    switch (subscription_type) {
        case SubscriptionType::HeadLineActuals:
            uri += "/headline-actuals";
            break;
        case SubscriptionType::ReleaseCalendar:
            uri += "/release-calendar";
            break;
        case SubscriptionType::HeadLineSurveys:
            uri += "/headline-surveys";
            break;
    }
    switch (topic_type) {
        case TopicType::Ticker:
            uri += "/ticker/";
            break;
        case TopicType::Cusip:
            uri += "/cusip/";
            break;
    }
    uri += topic;
    if (!options.empty()) {
        uri += "?" + options;
    }
    return uri;
}

int Context::subscribe(SubscriptionRequest& request) {
    json j = newLogMessage(module_name);
    j["result"] = "error";
    if (!session_) {
        j["message"] = "Session not initialized";
        logger_.log(j.dump());
        return -1;
    }
    if (request.topic.empty()) {
        j["message"] = "Topic is empty";
        logger_.log(j.dump());
        return -1;
    }
    blpapi::SubscriptionList sub;
    blpapi::CorrelationId corr_id(++subscription_counter_);
    std::string reference = service_ + request.toUri();
    try {
    sub.add(reference.c_str(), corr_id);
    session_->subscribe(sub);
    } catch (const blpapi::Exception& e) {
        j["message"] = "Subscription failed";
        j["error"] = e.description();
        logger_.log(j.dump());
        return -1;
    }
    return 0;

}

}
