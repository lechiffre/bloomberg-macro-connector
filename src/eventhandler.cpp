#include <unistd.h>
#include <iostream>
#include <sstream>
#include <flatbuffers/flatbuffer_builder.h>
#include <boost/format.hpp>
#include "blpconn_event.h"
#include "blpconn_message.h"
#include "blpconn_deserialize.h"


namespace BlpConn {

static const uint8_t module = static_cast<uint8_t>(Module::System);

static const blpapi::Name SESSION_CONNECTION_UP("SessionConnectionUp");
static const blpapi::Name SESSION_STARTED("SessionStarted");
static const blpapi::Name SESSION_CONNECTION_DOWN("SessionConnectionDown");
static const blpapi::Name SESSION_TERMINATED("SessionTerminated");
static const blpapi::Name SERVICE_OPENED("ServiceOpened");
static const blpapi::Name SUBSCRIPTION_STARTED("SubscriptionStarted");
static const blpapi::Name SUBSCRIPTION_STREAMS_ACTIVATED("SubscriptionStreamsActivated");
static const blpapi::Name SUBSCRIPTION_TERMINATED("SubscriptionTerminated");
static const blpapi::Name SUBSCRIPTION_FAILURE("SubscriptionFailure");
static const blpapi::Name ECONOMIC_EVENT("EconomicEvent");
static const blpapi::Name HEADLINE_ECONOMIC_EVENT("HeadlineEconomicEvent");
static const blpapi::Name HEADLINE_CALENDAR_EVENT("HeadlineCalendarEvent");

static void sendNotification(flatbuffers::FlatBufferBuilder& builder, Logger *logger) {
    uint8_t * buffer = builder.GetBufferPointer();
    int size = builder.GetSize();
    logger->notify(buffer, size);
}

void processEconomicEvent(const blpapi::Element& elem, Logger& logger) {
    if (elem.name() == HEADLINE_ECONOMIC_EVENT) {
#ifdef DEBUG
        uint64_t event_id = 0;
        if (logger.testing_) {
            event_id = logger.profiler_.push("EventHandler", "processEconomicEvent", "HeadlineEconomicEvent");
        }
#endif
        HeadlineEconomicEvent event = parseHeadlineEconomicEvent(elem);
        flatbuffers::FlatBufferBuilder builder = buildBufferEconomicEvent(event);
        // flatbuffers::FlatBufferBuilder builder = buildBufferEconomicEvent(elem);
        sendNotification(builder, &logger);
#ifdef DEBUG
        if (logger.testing_) {
            logger.profiler_.push("EventHandler", "processEconomicEvent", "HeadlineEconomicEvent", event_id); 
        }
#endif
    } else if (elem.name() == HEADLINE_CALENDAR_EVENT) {
#ifdef DEBUG
        uint64_t event_id = 0;
        if (logger.testing_) {
            event_id = logger.profiler_.push("EventHandler", "processEconomicEvent", "HeadlineCalendarEvent");
        }
#endif
        HeadlineCalendarEvent event = parseHeadlineCalendarEvent(elem);
        flatbuffers::FlatBufferBuilder builder = buildBufferCalendarEvent(event);
        // flatbuffers::FlatBufferBuilder builder = buildBufferCalendarEvent(elem);
        sendNotification(builder, &logger);
#ifdef DEBUG
        if (logger.testing_) {
            logger.profiler_.push("EventHandler", "processEconomicEvent", "HeadlineCalendarEvent", event_id); 
        }
#endif
    } else if (elem.name() == HEADLINE_CALENDAR_EVENT) {
    } else {
        std::string e = "Unknown event type: ";
        e += elem.name().string();
        logger.log(module, 0, 0, e);
    }
}

bool processSubscriptionData(const blpapi::Event& event, blpapi::Session *session, Logger& logger) {
#ifdef DEBUG
    uint64_t event_id = 0;
    if (logger.testing_) {
        event_id = logger.profiler_.push("EventHandler", "processSubscriptionData", "SubscriptionData");
    }
#endif
    blpapi::MessageIterator msgIter(event);
    while (msgIter.next()) {
        blpapi::Message msg = msgIter.message();
        blpapi::Element elem = msg.asElement();
        if (elem.name() == ECONOMIC_EVENT) {
            for (std::size_t i = 0; i < elem.numValues(); ++i) {
                blpapi::Element sub_elem = elem.getElement(i);
                processEconomicEvent(sub_elem, logger);
            }
        } else {
            uint64_t correlation_id = msg.correlationId().asInteger();
            logger.log(
                static_cast<uint8_t>(Module::Heartbeat),
                0,
                correlation_id,
                "Subscription Heartbeat");
        }
    }
#ifdef DEBUG
    if (logger.testing_) {
        logger.profiler_.push("EventHandler", "processSubscriptionData", "SubscriptionData", event_id); 
    }
#endif
    return true;
}

bool processSessionStatus(const blpapi::Event& event, blpapi::Session *session, Logger& logger) {
#ifdef DEBUG
    uint64_t event_id = 0;
    if (logger.testing_) {
        event_id = logger.profiler_.push("EventHandler", "processSessionStatus", "SessionStatus");
    }
#endif
    blpapi::MessageIterator msgIter(event);
    const uint8_t module = static_cast<uint8_t>(Module::Session);
    while (msgIter.next()) {
        blpapi::Message msg = msgIter.message();
        blpapi::Element elem = msg.asElement();
        std::ostringstream oss;
        oss << elem;
        if (elem.name() == SESSION_CONNECTION_UP) {
            logger.log(module, static_cast<uint8_t>(SessionStatus::ConnectionUp), 0, oss.str());
        } else if (elem.name() == SESSION_STARTED) {
            logger.log(module, static_cast<uint8_t>(SessionStatus::Started), 0, oss.str());
        } else if (elem.name() == SESSION_CONNECTION_DOWN) {
            logger.log(module, static_cast<uint8_t>(SessionStatus::ConnectionDown), 0, oss.str());
        } else if (elem.name() == SESSION_TERMINATED) {
            logger.log(module, static_cast<uint8_t>(SessionStatus::Terminated), 0, oss.str());
        } else {
            logger.log(module, static_cast<uint8_t>(SessionStatus::Unknown), 0, oss.str());
        }
    }
#ifdef DEBUG
    if (logger.testing_) {
        logger.profiler_.push("EventHandler", "processSessionStatus", "SessionStatus", event_id); 
    }
#endif
    return true;
}

bool processServiceStatus(const blpapi::Event& event, blpapi::Session *session, Logger& logger) {
#ifdef DEBUG
    uint64_t event_id = 0;
    if (logger.testing_) {
        event_id = logger.profiler_.push("EventHandler", "processServiceStatus", "ServiceStatus");
    }
#endif
    blpapi::MessageIterator msgIter(event);
    const uint8_t module = static_cast<uint8_t>(Module::Service);
    while (msgIter.next()) {
        blpapi::Message msg = msgIter.message();
        blpapi::Element elem = msg.asElement();
        std::ostringstream oss;
        oss << elem;
        if (elem.name() == SERVICE_OPENED) {
            logger.log(module, static_cast<uint8_t>(ServiceStatus::Opened), 0, oss.str());
        } else {
            logger.log(module, static_cast<uint8_t>(ServiceStatus::Unknown), 0, oss.str());
        }
    }
#ifdef DEBUG
    if (logger.testing_) {
        logger.profiler_.push("EventHandler", "processServiceStatus", "ServiceStatus", event_id); 
    }
#endif
    return true;
}

bool processSubscriptionStatus(const blpapi::Event& event, blpapi::Session *session, Logger& logger) {
#ifdef DEBUG
    uint64_t event_id = 0;
    if (logger.testing_) {
        event_id = logger.profiler_.push("EventHandler", "processSubscriptionStatus", "SubscriptionStatus");
    }
#endif
    blpapi::MessageIterator msgIter(event);
    const uint8_t module = static_cast<uint8_t>(Module::Subscription);
    while (msgIter.next()) {
        blpapi::Message msg = msgIter.message();
        uint64_t correlation_id = msg.correlationId().asInteger();
        blpapi::Element elem = msg.asElement();
        std::ostringstream oss;
        oss << elem;
        if (elem.name() == SUBSCRIPTION_STARTED) {
            logger.log(module, static_cast<uint8_t>(SubscriptionStatus::Started), correlation_id, oss.str());
        } else if (elem.name() == SUBSCRIPTION_STREAMS_ACTIVATED) {
            logger.log(module, static_cast<uint8_t>(SubscriptionStatus::StreamsActivated), correlation_id, oss.str());
        } else if (elem.name() == SUBSCRIPTION_TERMINATED) {
            logger.log(module, static_cast<uint8_t>(SubscriptionStatus::Terminated), correlation_id, oss.str());
        } else if (elem.name() == SUBSCRIPTION_FAILURE) {
            logger.log(module, static_cast<uint8_t>(SubscriptionStatus::Failure), correlation_id, oss.str());
        } else {
            logger.log(module, static_cast<uint8_t>(SubscriptionStatus::Unknown), correlation_id, oss.str());
        }
    }
#ifdef DEBUG
    if (logger.testing_) {
        logger.profiler_.push("EventHandler", "processSubscriptionStatus", "SubscriptionStatus", event_id); 
    }
#endif
    return true;
}

bool EventHandler::processEvent(const blpapi::Event& event, blpapi::Session *session) {
    switch(event.eventType()) {
        case blpapi::Event::SUBSCRIPTION_DATA:
            return processSubscriptionData(event, session, logger_);
        case blpapi::Event::SESSION_STATUS:
            return processSessionStatus(event, session, logger_);
        case blpapi::Event::SERVICE_STATUS:
            return processServiceStatus(event, session, logger_);
        case blpapi::Event::SUBSCRIPTION_STATUS:
            return processSubscriptionStatus(event, session, logger_);
        default:
            std::cout << "#### Unhandled event type: " << event.eventType() << std::endl;
            blpapi::MessageIterator msg_iter(event);
            while (msg_iter.next()) {
                blpapi::Message msg = msg_iter.message();
                std::ostringstream oss;
                oss << msg;
                logger_.log(
                    static_cast<uint8_t>(Module::Heartbeat),
                    0,
                    0,
                    oss.str());
            }
            return false;
    }
}

} // namespace BlpConn
