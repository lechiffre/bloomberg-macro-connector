#ifndef _BLPCONN_LOGGER_H
#define _BLPCONN_LOGGER_H

#include <iostream>
#include <string>
#include <vector>

#include "blpconn_observer.h"

namespace BlpConn {

class EventHandler;
class Context;

/**
 * This class is responsible for logging messages. It can log messages
 * to a specified output stream (default is std::cout) or to the collection
 * of registered observer functions.
 */
class Logger {
   public:
    friend class EventHandler; 
    friend class Context;
    /**
     * The default output stream is std::cout. The client program can
     * specify a different output stream if needed. The output stream
     * is used when no observer functions are registered. Otherwise,
     * notifications are sent only to the observer functions.
     */
    Logger(std::ostream* out_stream = &std::cout) : out_stream_(out_stream) {}

    /**
     * This method is the way to register observer functions.
     * The client program can register one or more observer functions.
     * These functions will receive JSON messages in string format.
     */
    void addNotificationHandler(ObserverFunc fnc) noexcept;

    /**
     * This method is used to log messages. It can be used to log
     * messages to the output stream or to the registered observer
     * functions.
     */
    void log(uint8_t module, uint8_t status, uint64_t correlation_id,
             const std::string& message);

    // void send_notification(Message message, MessageType msg_type);
    // void sendNotification(flatbuffers::FlatBufferBuilder& builder);
    /**
     * Notifies to all registered observer functions.
     */
    void notify(const uint8_t* buffer, size_t size);

   private:
    std::ostream* out_stream_;
    std::vector<ObserverFunc> callbacks_;
};

}  // namespace BlpConn

#endif  // _BLPCONN_LOGGER_H
