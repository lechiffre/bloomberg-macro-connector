/**
 * blpconn Library
 *
 * This library provides a C++ interface to connect to Bloomberg's services.
 * It is based on the Bloomberg C++ API and provides functionality to subscribe
 * to various Bloomberg data feeds, handle events, and log messages. This
 * library is focused on the Economic Data service. It provides an interface to
 * open an mantain a connection to the Bloomberg service, subscribe to data
 * feeds, and handle incoming events by user defined callback functions.
 */

#ifndef _BLPCONN_H
#define _BLPCONN_H

#include <blpconn_event.h>

using namespace BloombergLP;

/**
 * The namespace for classes and functions in this library is
 * blpconn.
 */
namespace BlpConn {

// TODO Subscription type will be removed
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
  Ticker, // Generic identifier
  Bbgid   // Bloomberg Id or FIGI
};

/**
 * This structure represents a subscription request. It includes default values
 * for the topic type and event type. The client program can specify the topic,
 * topic type, event type, and options when creating a subscription request.
 *
 * Fields:
 *
 * topic: Requested topic, generally a financial instrument. Example:
 * "CATBTOTB Index"
 *
 * topic_type: It indicates the standard or form used to represent the
 * topic.Check TopicType enum to see possible values subscription_type
 *
 * options: additional parameters passed as options
 *
 * correlation_id: a integer representing an id for the subscription. This
 * id can be set by the client. Once it is set, it can be used to cancel
 * the subscritpion.
 */
struct SubscriptionRequest {
  // std::string service;
  std::string topic;
  TopicType topic_type = TopicType::Ticker;
  std::string options = "";
  uint64_t correlation_id = 0;

  /**
   * Converts struct attributes to an URI following standard
   * defined by Bloomberg. Example:
   *
   *   "//blpapi/macro-indicators/ticker/CATBTOTB Index"
   */
  std::string toUri() const;
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
  Context() {
  }

  /**
   * In the case the service is still active, the destructor
   * take care to shut it down
   */
  ~Context() {
    if (session_) {
      shutdownSession();
    }
  }

  /**
   * Authentication and connection to Bloomberg service.  The configuration
   * file contains connection parameters and authentication information. The
   * client program should specify the path to the configuration file.  The
   * connection process is notified by the event handler.  Therefore, in
   * order to receive notifications by callback functions, those functions
   * should be registered before calling this method.
   *
   * @param config_path The path to the configuration file.
   * @return true if the connection is successful, false otherwise.
   */
  bool initializeSession(const std::string &config_path);

  /**
   * This method disconnects from the Bloomberg service.
   * It is automatically called by the constructor.
   */
  void shutdownSession();

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
  int subscribe(SubscriptionRequest &request);

  /**
   * To unscribe the session for a data feed. It should include a
   * correlation id set during the subscription, in order to identify
   * the subscription to be closed.
   *
   * @param request The subscription request. The correlation id should
   * be the same used for the subscription.
   */
  void unsubscribe(SubscriptionRequest &request);

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

  /**
   * The client program can use this method to log own messages.
   * Message should be in JSON format and be passed as strings.
   */
  void log(uint8_t module, uint8_t status, uint64_t correlation_id,
           const std::string &message) {
    event_handler_.logger_.log(module, status, correlation_id, message);
  }

private:
  std::string service_ = "//blp/economic-data";
  EventHandler event_handler_;
  blpapi::Session *session_ = nullptr;
  int subscription_counter_ = 0;
};

} // namespace BlpConn

#endif // _BLPCONN_H
