#include <flatbuffers/flatbuffer_builder.h>
#include <unistd.h>
#include <iostream>
#include "blpconn.h"
#include "economic_event.h"

namespace BlpConn {

static const blpapi::Name ECONOMIC_EVENT("EconomicEvent");
static const blpapi::Name HEADLINE_ECONOMIC_EVENT("HeadlineEconomicEvent");
static const blpapi::Name HEADLINE_CALENDAR_EVENT("HeadlineCalendarEvent");

void processEconomicEvent(const blpapi::Element& elem) {
    if (elem.name() == HEADLINE_ECONOMIC_EVENT) {
        HeadlineEconomicEvent message = parseHeadlineEconomicEvent(elem);
        std::cout << ">>> HeadlineEconomicEvent: " << message << std::endl;
    } else if (elem.name() == HEADLINE_CALENDAR_EVENT) {
        HeadlineCalendarEvent message = parseHeadlineCalendarEvent(elem);
        std::cout << ">>> HeadlineCalendarEvent: " << message << std::endl;
    } else {
        std::cout << ">>> Unknown event type: " << elem.name() << std::endl;
    }
}

bool processSubscriptionData(const blpapi::Event& event, blpapi::Session *session) {
    blpapi::MessageIterator msgIter(event);
    while (msgIter.next()) {
        blpapi::Message msg = msgIter.message();
        blpapi::Element elem = msg.asElement();
        if (elem.name() == ECONOMIC_EVENT) {
            for (std::size_t i = 0; i < elem.numValues(); ++i) {
                std::cout << ">>> Economic Event: " << i << std::endl;
                blpapi::Element sub_elem = elem.getElement(i);
                processEconomicEvent(sub_elem);
            }
        }
    }
    return true;
}

bool processSessionStatus(const blpapi::Event& event, blpapi::Session *session) {
    blpapi::MessageIterator msgIter(event);
    while (msgIter.next()) {
        blpapi::Message msg = msgIter.message();
        std::cout << msg << std::endl;
        blpapi::Element elem = msg.asElement();
        std::cout << elem << std::endl;
    }
    return true;
}

bool processSubscriptionStatus(const blpapi::Event& event, blpapi::Session *session) {
    blpapi::MessageIterator msgIter(event);
    while (msgIter.next()) {
        blpapi::Message msg = msgIter.message();
        std::cout << msg << std::endl;
        blpapi::Element elem = msg.asElement();
        std::cout << elem << std::endl;
    }
    return true;
}

bool EventHandler::processEvent(const blpapi::Event& event, blpapi::Session *session) {
    switch(event.eventType()) {
        case blpapi::Event::SUBSCRIPTION_STATUS:
            return processSubscriptionStatus(event, session);
        case blpapi::Event::SUBSCRIPTION_DATA:
            return processSubscriptionData(event, session);
        case blpapi::Event::RESPONSE:
            return processSessionStatus(event, session);
        default:
            std::cout << "Unhandled event type: " << event.eventType() << std::endl;
            return false;
    }
}

}
