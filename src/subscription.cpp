#include <cstring>
#include "blpconn.h"
#include "blpconn_message.h"


namespace BlpConn {

static const char* toString(TopicType topic_type) noexcept {
    switch (topic_type) {
        case TopicType::Ticker:
            return "ticker";
        case TopicType::Bbgid:
            return "bbgid";
    }
    return "unknown";
}

// TODO Test
std::string SubscriptionRequest::toUri() const {
    const char* topic_type_str = toString(topic_type);
    size_t len = 2 + std::strlen(topic_type_str) + topic.length() +
        (!options.empty() ? 1 + options.length() : 0);
    std::string uri;
    uri.reserve(len);
    uri += "/";
    uri.append(topic_type_str);
    uri += "/";
    uri.append(topic);
    if (!options.empty()) {
        uri += "?";
        uri.append(options);
    }
    return uri;
}

// TODO test
int Context::subscribe(SubscriptionRequest& request) {
    PROFILE_FUNCTION()
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
    END_PROFILE_FUNCTION()
    return subscription_counter_++;
}

// TODO test
void Context::unsubscribe(SubscriptionRequest& request) {
    PROFILE_FUNCTION()
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
    END_PROFILE_FUNCTION()
}

} // namespace BlpConn
