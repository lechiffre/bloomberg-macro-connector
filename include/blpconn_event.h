#ifndef _BLPCONN_EVENT_H
#define _BLPCONN_EVENT_H

#include "blpconn_logger.h"
#include <blpapi_session.h>

using namespace BloombergLP;

namespace BlpConn {

// Forward declaration
class Context;

/**
 * A class to handle Bloomberg events, as well as event generated
 * directly by this library. This class receives event notifications,
 * formats them as JSON messages, and by the logger object, sends
 * them to the registered observer functions or to the output stream.
 * Part of the notifications are heartbeats, which are generated
 * to keep the connection alive and report the status of the connection.
 */
class EventHandler : public blpapi::EventHandler {
public:
  friend Context;

  /**
   * This method is called when an event is received. It processes
   * the event and sends the formatted JSON message to the logger.
   * This method is call by the Bloomberg API when an event is received.
   */
  bool processEvent(const blpapi::Event &event,
                    blpapi::Session *session) override;

private:
  Logger logger_;
};

} // namespace BlpConn

#endif // _BLPCONN_EVENT_H
