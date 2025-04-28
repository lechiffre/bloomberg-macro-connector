#include "blpconn.h"

namespace BlpConn {

bool EventHandler::processEvent(const blpapi::Event& event, blpapi::Session *session) {
    // json j = newLogMessage(event.eventType());
    blpapi::MessageIterator msgIter(event);
    while (msgIter.next()) {
        blpapi::Message msg = msgIter.message();
        std::cout << msg << std::endl;
        /*
        std::string service = msg.service();
        j["service"] = service;
        j["message_type"] = msg.messageType();
        j["correlation_id"] = msg.correlationId();
        j["num_elements"] = msg.numElements();
        */
        /*
        std::string raw = msg;
        j["raw"] = raw;
        */
    }
    return true;
}

}


