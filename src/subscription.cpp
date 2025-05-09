#include "blpconn.h"
#include "blpconn_message.h"


namespace BlpConn {

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
    blpapi::CorrelationId corr_id(request.correlation_id);
    if (!session_) {
        log(
            static_cast<uint8_t>(Module::Session),
            static_cast<uint8_t>(SessionStatus::ConnectionDown),
            corr_id.asInteger(),
            "Session not initialized");
        return -1;
    }
    if (request.topic.empty()) {
        log(
            static_cast<uint8_t>(Module::Subscription),
            static_cast<uint8_t>(SubscriptionStatus::Failure),
            corr_id.asInteger(),
            "Topic cannot be empty");
        return -1;
    }
    blpapi::SubscriptionList sub;
    std::string reference = service_ + request.toUri();
    try {
        sub.add(reference.c_str(), corr_id);
        session_->subscribe(sub);
    } catch (const blpapi::Exception& e) {
        log(
            static_cast<uint8_t>(Module::Subscription),
            static_cast<uint8_t>(SubscriptionStatus::Failure),
            corr_id.asInteger(),
            "Error: Subscription failed");
        return -1;
    }
    log(
        static_cast<uint8_t>(Module::Subscription),
        static_cast<uint8_t>(SubscriptionStatus::Success),
        corr_id.asInteger(),
        "Subscription successful");
    return subscription_counter_++;
}

void Context::unsubscribe(SubscriptionRequest& request) {
    blpapi::CorrelationId corr_id(request.correlation_id);
    if (!session_) {
        log(
            static_cast<uint8_t>(Module::Session),
            static_cast<uint8_t>(SessionStatus::ConnectionDown),
            corr_id.asInteger(),
            "Session not initialized");
        return;
    }
    if (request.topic.empty()) {
        log(
            static_cast<uint8_t>(Module::Subscription),
            static_cast<uint8_t>(SubscriptionStatus::Failure),
            corr_id.asInteger(),
            "Topic cannot be empty");
        return;
    }
    blpapi::SubscriptionList sub;
    std::string reference = service_ + request.toUri();
    try {
        sub.add(reference.c_str(), corr_id);
        session_->unsubscribe(sub);
    } catch (const blpapi::Exception& e) {
        log(
            static_cast<uint8_t>(Module::Subscription),
            static_cast<uint8_t>(SubscriptionStatus::Failure),
            corr_id.asInteger(),
            "Error: Unsubscription failed");
        return;
    }
}

} // namespace BlpConn
