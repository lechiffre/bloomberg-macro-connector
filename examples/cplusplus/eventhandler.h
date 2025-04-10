#ifndef _EVENTHANDLER_H
#define _EVENTHANDLER_H

#include <blpapi_session.h>

using namespace BloombergLP::blpapi;

class TickersEventHandler : public EventHandler {
    public:
        bool processEvent(const Event& event, Session *session) override;
};

#endif // _EVENTHANDLER_H
