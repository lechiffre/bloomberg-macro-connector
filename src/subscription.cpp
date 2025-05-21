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
        case TopicType::Sedol:
            uri += "/sedol/";
            break;
        case TopicType::Isin:
            uri += "/isin/";
            break;
        case TopicType::Bsid:
            uri += "/bsid/";
            break;
        case TopicType::Buid:
            uri += "/buid/";
            break;
        case TopicType::Eid:
            uri += "/eid/";
            break;
        case TopicType::Figi:   
            uri += "/figi/";
            break;
        default:
            uri += "/ticker/";
            break;
    }
    uri += topic;
    if (!options.empty()) {
        uri += "?" + options;
    }
    return uri;
}

int Context::subscribe(SubscriptionRequest& request) {
#ifdef DEBUG
    uint64_t event_id = 0;
    if (event_handler_.logger_.testing_) {
        event_id = event_handler_.logger_.profiler_.push("Context", "subscribe", "SubscriptionRequest");
    }
#endif
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
#ifdef DEBUG
    if (event_handler_.logger_.testing_) {
        event_handler_.logger_.profiler_.push("Context", "subscribe", "SubscriptionRequest", event_id);
    }
#endif
}

void Context::unsubscribe(SubscriptionRequest& request) {
#ifdef DEBUG
    uint64_t event_id = 0;
    if (event_handler_.logger_.testing_) {
        event_id = event_handler_.logger_.profiler_.push("Context", "unsubscribe", "UnsubscriptionRequest");
    }
#endif
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
#ifdef DEBUG
    if (event_handler_.logger_.testing_) {
        event_handler_.logger_.profiler_.push("Context", "unsubscribe", "UnsubscriptionRequest", event_id);
    }
#endif
}

} // namespace BlpConn
