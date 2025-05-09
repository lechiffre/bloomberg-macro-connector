#ifndef ECONOMIC_EVENT_H
#define ECONOMIC_EVENT_H

#include <string>
#include <cmath>
#include <blpapi_datetime.h>
#include <blpapi_element.h>
#include <flatbuffers/flatbuffers.h>
// #include "blpconn_fb_generated.h"

using namespace BloombergLP;

namespace BlpConn {

enum Modules {
    ModuleUnknown = 0,
    ModuleSystem,
    ModuleSession,
    ModuleSubscription,
    ModuleService,
    ModuleHearbeat,
    ModuleAnother = 99,
};

enum SessionStatus {
    SessionUnknown = 0,
    SessionConnectionUp,
    SessionStarted,
    SessionConnectionDown,
    SessionTerminated,
    SessionAnother = 99,
};

enum class ReleaseStatus {
    Unknown = 0,
    Released,
    Scheduled,
    Another = 99,
};

enum class EventSubType {
    Unknown = 0,
    New,
    Update,
    Unitpaint,
    Delete,
    Another = 99,
};

enum class EventType {
    Unknown = 0,
    Actual,
    Revision,
    Estimate,
    Calendar,
    Another = 99,
};

struct DateTimeType {
    uint64_t microseconds = 0;
    short offset = 0;
};

struct LogMessage {
    DateTimeType log_dt;
    int module = 0;
    int status = 0;
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
    int event_id = 0;
    std::string observation_period = "";
    DateTimeType release_start_dt;
    DateTimeType release_end_dt;
};

struct HeadlineEconomicEvent: public HeadlineBaseEvent {
    ValueType value;
    ValueType prior_value;
    int prior_event_id = 0;
    std::string prior_observation_period = "";
    DateTimeType prior_economic_release_start_dt;
    DateTimeType prior_economic_release_end_dt;
};

struct HeadlineCalendarEvent: public HeadlineBaseEvent {
    ReleaseStatus release_status = ReleaseStatus::Unknown;
};


} // namespace BlpConn

#endif // ECONOMIC_EVENT_H
