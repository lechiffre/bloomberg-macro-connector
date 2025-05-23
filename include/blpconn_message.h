#ifndef ECONOMIC_EVENT_H
#define ECONOMIC_EVENT_H

#include <blpapi_datetime.h>
#include <blpapi_element.h>

#include <cmath>
#include <cstdint>
#include <string>
// #include <flatbuffers/flatbuffers.h>

using namespace BloombergLP;

namespace BlpConn {

enum class Module : uint8_t {
  Unknown = 0,
  System,
  Session,
  Subscription,
  Service,
  Heartbeat,
  Another = 99,
};

enum class SessionStatus : uint8_t {
  Unknown = 0,
  ConnectionUp,
  Started,
  ConnectionDown,
  Terminated,
  InvalidOptions,
  Failure,
  Another = 99,
};

enum class SubscriptionStatus : uint8_t {
  Unknown = 0,
  Started,
  StreamsActivated,
  Terminated,
  Success,
  Failure,
  Another = 99,
};

enum class ServiceStatus : uint8_t {
  Unknown = 0,
  Opened,
  Closed,
  Failure,
  Another = 99,
};

enum class ReleaseStatus : uint8_t {
  Unknown = 0,
  Released,
  Scheduled,
  Another = 99,
};

enum class EventSubType : uint8_t {
  Unknown = 0,
  New,
  Update,
  Unitpaint,
  Delete,
  Another = 99,
};

enum class EventType : uint8_t {
  Unknown = 0,
  Actual,
  Revision,
  Estimate,
  Calendar,
  Another = 99,
};

struct DateTimeType {
  uint64_t microseconds = 0;
  uint16_t offset = 0;
};

struct LogMessage {
  DateTimeType log_dt;
  uint8_t module = 0;
  uint8_t status = 0;
  uint64_t correlation_id = 0;
  std::string message;
};

struct ValueType {
  double number = std::nanf("");
  double value = std::nanf("");
  double low = std::nanf("");
  double high = std::nanf("");
  double median = std::nanf("");
  double average = std::nanf("");
  double standard_deviation = std::nanf("");
};

struct HeadlineBaseEvent {
  std::string id_bb_global;
  std::string parsekyable_des;
  std::string description = "";
  EventType event_type = EventType::Unknown;
  EventSubType event_subtype = EventSubType::Unknown;
  uint64_t event_id = 0;
  std::string observation_period = "";
  DateTimeType release_start_dt;
  DateTimeType release_end_dt;
};

struct HeadlineEconomicEvent : public HeadlineBaseEvent {
  ValueType value;
  ValueType prior_value;
  uint64_t prior_event_id = 0;
  std::string prior_observation_period = "";
  DateTimeType prior_economic_release_start_dt;
  DateTimeType prior_economic_release_end_dt;
};

struct HeadlineCalendarEvent : public HeadlineBaseEvent {
  ReleaseStatus release_status = ReleaseStatus::Unknown;
};

} // namespace BlpConn

#endif // ECONOMIC_EVENT_H
