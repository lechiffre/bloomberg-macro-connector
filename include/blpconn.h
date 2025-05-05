/**
 * blpconn Library
 *
 * This library provides a C++ interface to connect to Bloomberg's services.
 * It is based on the Bloomberg C++ API and provides functionality to subscribe to
 * various Bloomberg data feeds, handle events, and log messages.
 * This library is focused on the Economic Data service. It provides an interface
 * to open an mantain a connection to the Bloomberg service, subscribe to data feeds,
 * and handle incoming events by user defined callback functions.
 */

#ifndef _BLPCONN_H
#define _BLPCONN_H

#include <cstdint>
#include <string>
#include <vector>
#include <iostream>
#include <blpapi_session.h>

using namespace BloombergLP;

/**
 * The namespace for classes and functions in this library is
 * blpconn.
 */
namespace BlpConn {

/**
 * Definition for observer functions. A client program can provide one or more
 * observer functions to handle serialized values. These functions will receive
 * the type of object being received, a pointer to the serialized buffer, and
 * its size.  The client program is responsible for parsing and processing the
 * data. Data is serialized using FlatBuffers.
 *
 * @param type The type of the object being received.
 * @param buffer A pointer to the serialized buffer.
 * @param size The size of the serialized buffer.
 */
typedef void (*ObserverFunc)(
    const uint8_t *buffer,
    size_t size
);


/**
 * The EventType enum defines the types of events that can be
 * subscribed to. The client program can choose one or more
 * event types when subscribing to a data feed.
 */
enum class SubscriptionType {
    HeadlineActuals,
    ReleaseCalendar,
    HeadlineSurveys
};

/**
 * Bloomberg's let subscriptors to request data using
 * different identification standards.
 */
enum class TopicType {
    Ticker,
    Cusip,
    // TODO: Add more types
};

/**
 * This structure represents a subscription request. It includes
 * default values for the topic type and event type. The client program
 * can specify the topic, topic type, event type, and options when
 * creating a subscription request.
 */
struct SubscriptionRequest {
    std::string topic;
    TopicType topic_type = TopicType::Ticker;
    SubscriptionType subscription_type = SubscriptionType::HeadlineActuals;
    std::string options = "";
    int correlection_id = 0;
    std::string toUri();
};

class Context;

/**
 * This class is responsible for logging messages. It can log messages
 * to a specified output stream (default is std::cout) or to the collection
 * of registered observer functions.
 */
class Logger {
public:
    /**
     * The default output stream is std::cout. The client program can
     * specify a different output stream if needed. The output stream
     * is used when no observer functions are registered. Otherwise,
     * notifications are sent only to the observer functions.
     */
    Logger(std::ostream* out_stream = &std::cout): out_stream_(out_stream) {}

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
    void log(const std::string& module_name, const std::string& message);

    // void send_notification(Message message, MessageType msg_type);
    // void sendNotification(flatbuffers::FlatBufferBuilder& builder);
    /**
     * Notifies to all registered observer functions.
     */
    void notify(const uint8_t* buffer, size_t size);

private:
    std::ostream* out_stream_;
    std::vector<ObserverFunc> callbacks_;
    // TODO: check if it needs to have one or more buffers
};

/**
 * A class to handle Bloomberg events, as well as event generated
 * directly by this library. This class receives event notifications,
 * formats them as JSON messages, and by the logger object, sends
 * them to the registered observer functions or to the output stream.
 * Part of the notifications are heartbeats, which are generated
 * to keep the connection alive and report the status of the connection.
 */
class EventHandler: public blpapi::EventHandler {
    public:
        /**
         * It is possible to set a custom logger object.
         */
        // void setLogger(Logger *prt_logger) { prt_logger_ = prt_logger; }
        // void setLogger2(Logger& logger) { logger_ = logger; }

        /**
         * This method is called when an event is received. It processes
         * the event and sends the formatted JSON message to the logger.
         * This method is call by the Bloomberg API when an event is received.
         */
        friend Context;
        bool processEvent(const blpapi::Event& event, blpapi::Session *session) override;

    private:
        void processEconomicEvent(const blpapi::Element& elem);
        bool processSubscriptionData(const blpapi::Event& event, blpapi::Session *session);
        bool processSessionStatus(const blpapi::Event& event, blpapi::Session *session);
        bool processSubscriptionStatus(const blpapi::Event& event, blpapi::Session *session);
        Logger logger_;
};

/**
 * Context is the main class of this library. It is expected that
 * client program only need to deal with this class. It provides methods
 * to initialize the connection to the Bloomberg service, subscribe to
 * data feeds, and handle events. It also provides a logger object
 * to log messages.
 */
class Context {
public:
    /**
     * Constructor. It initializes the logger and event handler objects.
     */
    Context() {
        // event_handler_.setLogger(&logger_);
    }

    /**
     * In the case the service is still active, the destructor
     * take care to shut it down
     */
    ~Context() {
        if (session_) {
            shutdownService();
        }
    }

    /**
     * Authentication and connection to Bloomberg service.
     * The configuration file contains connection parameters
     * and authentication information. The client program should
     * specify the path to the configuration file.
     * The connection process is notified by the event handler.
     * Therefore, in order to receive notifications by callback
     * functions, those functions should be registered before
     * calling this method.
     *
     * @param config_path The path to the configuration file.
     * @return true if the connection is successful, false otherwise.
     */
    bool initializeService(std::string& config_path);

    /**
     * This method disconnects from the Bloomberg service.
     */
    void shutdownService();

    /**
     * To report if the connection with Bloomberg service is
     * established.
     *
     * @return true if the connection is established, false otherwise.
     */
    bool isConnected() { return session_ != nullptr; }

    /**
     * This method subscribes to a data feed. The client program
     * should specify the topic, topic type, event type, and options
     * in the subscription request. The subscription request is
     * passed by reference.
     *
     * @param request The subscription request.
     * @return 0 if the subscription is successful, -1 otherwise.
     */
    int subscribe(SubscriptionRequest& request);

    void unsubscribe(SubscriptionRequest& request);

    /**
     * To register observer functions. The client program can
     * register one or more observer functions. These functions
     * will receive JSON messages in string format. The client
     * program is responsible to parse and process the JSON messages.
     *
     * @param fnc The observer function to register.
     */
    void addNotificationHandler(ObserverFunc fnc) noexcept {
        event_handler_.logger_.addNotificationHandler(fnc);
    }

    /*
    void sendNotification(flatbuffers::FlatBufferBuilder& builder) {
        event_handler_.logger_.sendNotification(builder);
    }
    */

    /**
     * The client program can use this method to log own messages.
     * Message should be in JSON format and be passed as strings.
     */
    void log(const std::string& module_name, const std::string& message) {
        event_handler_.logger_.log(module_name, message);
    }

private:
    std::string service_ = "//blp/economic-data";
    EventHandler event_handler_;
    blpapi::Session *session_ = nullptr;
    int subscription_counter_ = 0;
};

}

#endif // _BLPCONN_H
