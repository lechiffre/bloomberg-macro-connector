#include "blpconn.h"
#include "blpconn_message.h"


namespace BlpConn {

static const int module = ModuleSystem;

const std::string SubscriptionRequest::toUri() {
    std::string uri = "";
    switch (subscription_type) {
        case SubscriptionType::HeadlineActuals:
            uri += "/headline-actuals";
            break;
        case SubscriptionType::ReleaseCalendar:
            uri += "/release-calendar";
            break;
        case SubscriptionType::HeadlineSurveys:
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
    if (!session_) {
        log(module, 0, "Error: Session not initialized");
        return -1;
    }
    if (request.topic.empty()) {
        log(module, 0, "Error: Topic cannot be empty");
        return -1;
    }
    blpapi::SubscriptionList sub;
    blpapi::CorrelationId corr_id(subscription_counter_);
    std::string reference = service_ + request.toUri();
    try {
        sub.add(reference.c_str(), corr_id);
        session_->subscribe(sub);
    } catch (const blpapi::Exception& e) {
        log(module, 0, "Error: Subscription failed");
        return -1;
    }
    return subscription_counter_++;

}

void Context::unsubscribe(SubscriptionRequest& request) {
    if (!session_) {
        log(module, 0, "Error: Session not initialized");
        return;
    }
    if (request.topic.empty()) {
        log(module, 0, "Error: Topic cannot be empty");
        return;
    }
    blpapi::SubscriptionList sub;
    std::string reference = service_ + request.toUri();
    try {
        sub.add(reference.c_str());
        session_->unsubscribe(sub);
    } catch (const blpapi::Exception& e) {
        log(module, 0, "Error: Subscription failed");
        return;
    }
}

} // namespace BlpConn
