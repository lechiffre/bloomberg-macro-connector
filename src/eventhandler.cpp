#include <flatbuffers/flatbuffer_builder.h>
#include "blpconn.h"
#include "economic_event_generated.h"

namespace BlpConn {

void processEconomicEvent(const blpapi::Element& elem) {
    flatbuffers::FlatBufferBuilder builder;
    if (elem.hasElement(blpapi::Name("HeadlineEconomicEvent"))) {

    } else if (elem.hasElement(blpapi::Name("HeadlineCalendarEvent"))) {
    }
}

bool processSubscriptionData(const blpapi::Event& event, blpapi::Session *session) {
    blpapi::MessageIterator msgIter(event);
    while (msgIter.next()) {
        blpapi::Message msg = msgIter.message();
        std::cout << ">>> Message : " << msg << std::endl;
        blpapi::Element elem = msg.asElement();
        std::cout << ">>> Element: " << elem << std::endl;
        std::cout << ">>> Name : " << elem.name() << std::endl;
        if (elem.name() == "EconomicEvent") {
            for (size_t i = 0; i < elem.numElements(); i++) {
                blpapi::Element child = elem.getElement(i);
                std::string name = child.name().string();
                std::cout << ">>> SubName: " << name << std::endl;
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