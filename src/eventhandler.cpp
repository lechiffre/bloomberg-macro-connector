#include "blpconn.h"

namespace BlpConn {

bool EventHandler::processEvent(const blpapi::Event& event, blpapi::Session *session) {
    // json j = newLogMessage(event.eventType());
    blpapi::MessageIterator msgIter(event);
    while (msgIter.next()) {
        blpapi::Message msg = msgIter.message();
        std::cout << msg << std::endl;
        blpapi::Element elem = msg.asElement();
        json j;
        parseElement(elem, j);
        std::cout << j.dump() << std::endl;
    }
    return true;
}

}


