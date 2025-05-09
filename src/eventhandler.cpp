#include <unistd.h>
#include <iostream>
#include <flatbuffers/flatbuffer_builder.h>
#include <boost/format.hpp>
#include "blpconn_event.h"
#include "blpconn_message.h"
#include "blpconn_deserialize.h"


namespace BlpConn {


static const int module = ModuleSystem;;

static const blpapi::Name ECONOMIC_EVENT("EconomicEvent");
static const blpapi::Name HEADLINE_ECONOMIC_EVENT("HeadlineEconomicEvent");
static const blpapi::Name HEADLINE_CALENDAR_EVENT("HeadlineCalendarEvent");

static void sendNotification(flatbuffers::FlatBufferBuilder& builder, Logger *logger) {
    uint8_t * buffer = builder.GetBufferPointer();
    int size = builder.GetSize();
    logger->notify(buffer, size);
}

void EventHandler::processEconomicEvent(const blpapi::Element& elem) {
    if (elem.name() == HEADLINE_ECONOMIC_EVENT) {
        HeadlineEconomicEvent event = parseHeadlineEconomicEvent(elem);
        flatbuffers::FlatBufferBuilder builder = buildBufferEconomicEvent(event);
        sendNotification(builder, &logger_);
    } else if (elem.name() == HEADLINE_CALENDAR_EVENT) {
        HeadlineCalendarEvent event = parseHeadlineCalendarEvent(elem);
        flatbuffers::FlatBufferBuilder builder = buildBufferCalendarEvent(event);
        sendNotification(builder, &logger_);
    } else {
        std::string e = "Unknown event type: ";
        e += elem.name().string();
        logger_.log(module, 0, e);
    }
}

bool EventHandler::processSubscriptionData(const blpapi::Event& event, blpapi::Session *session) {
    blpapi::MessageIterator msgIter(event);
    while (msgIter.next()) {
        blpapi::Message msg = msgIter.message();
        std::cout << "### Message " << msg << std::endl;
        blpapi::Element elem = msg.asElement();
        if (elem.name() == ECONOMIC_EVENT) {
            for (std::size_t i = 0; i < elem.numValues(); ++i) {
                blpapi::Element sub_elem = elem.getElement(i);
                processEconomicEvent(sub_elem);
                std::cout << elem << std::endl;
            }
        } else {
            std::cout << "### Not processed subscription data: " << elem << std::endl;
        }
    }
    return true;
}

bool EventHandler::processSessionStatus(const blpapi::Event& event, blpapi::Session *session) {
    blpapi::MessageIterator msgIter(event);
    while (msgIter.next()) {
        blpapi::Message msg = msgIter.message();
        std::cout << msg << std::endl;
        blpapi::Element elem = msg.asElement();
        std::cout << "### " << elem << std::endl;
    }
    return true;
}

bool EventHandler::processSubscriptionStatus(const blpapi::Event& event, blpapi::Session *session) {
    blpapi::MessageIterator msgIter(event);
    while (msgIter.next()) {
        blpapi::Message msg = msgIter.message();
        blpapi::Element elem = msg.asElement();
        std::cout << "### " << elem << std::endl;
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
            std::cout << "#### Unhandled event type: " << event.eventType() << std::endl;
            blpapi::MessageIterator msg_iter(event);
            while (msg_iter.next()) {
                blpapi::Message msg = msg_iter.message();
                std::cout << "### Message " << msg << std::endl;
                blpapi::Element elem = msg.asElement();
                std::cout << elem << std::endl;
            }
            
            return false;
    }
}

}
