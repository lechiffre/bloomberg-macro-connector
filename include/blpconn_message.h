#ifndef ECONOMIC_EVENT_H
#define ECONOMIC_EVENT_H

#include <cstdint>
#include <string>
#include <cmath>
#include <blpapi_datetime.h>
#include <blpapi_element.h>
#include <ostream>
#include <flatbuffers/flatbuffers.h>
#include "blpconn_fb_generated.h" // Generated FlatBuffers header

using namespace BloombergLP;

namespace BlpConn {

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
    std::string module_name;
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


enum class MessageType {
    LogMessage = 0,
    EconomicEvent,
    CalendarEvent
};



HeadlineCalendarEvent parseHeadlineCalendarEvent(const blpapi::Element& elem);
HeadlineEconomicEvent parseHeadlineEconomicEvent(const blpapi::Element& elem);

DateTimeType convertToDateTime(const blpapi::Datetime& dt);

EventType stringToEventType(const std::string& s);
std::string eventTypeToString(EventType t);

EventSubType stringToEventSubType(const std::string& s);
std::string eventSubTypeToString(EventSubType t);

ReleaseStatus stringToReleaseStatus(const std::string& s);
std::string releaseStatusToString(ReleaseStatus status);

std::ostream& operator<<(std::ostream& os, const DateTimeType& dt);
std::ostream& operator<<(std::ostream& os, const ValueType& value);
std::ostream& operator<<(std::ostream& os, const HeadlineBaseEvent& event);
std::ostream& operator<<(std::ostream& os, const HeadlineEconomicEvent& event);
std::ostream& operator<<(std::ostream& os, const HeadlineCalendarEvent& event);
std::ostream& operator<<(std::ostream& os, const LogMessage& log_message);

// Serialize functions
flatbuffers::Offset<FB::HeadlineEconomicEvent> serializeHeadlineEconomicEvent(
    flatbuffers::FlatBufferBuilder& builder, const HeadlineEconomicEvent& event);

flatbuffers::Offset<FB::HeadlineCalendarEvent> serializeHeadlineCalendarEvent(
    flatbuffers::FlatBufferBuilder& builder, const HeadlineCalendarEvent& event);

flatbuffers::Offset<FB::LogMessage> serializeLogMessage(
    flatbuffers::FlatBufferBuilder& builder, const LogMessage& log_message);

// Deserialize functions
HeadlineEconomicEvent toHeadlineEconomicEvent(const BlpConn::FB::HeadlineEconomicEvent* fb_event);

HeadlineCalendarEvent toHeadlineCalendarEvent(const BlpConn::FB::HeadlineCalendarEvent* fb_event);

LogMessage toLogMessage(const BlpConn::FB::LogMessage* fb_log_message);

flatbuffers::FlatBufferBuilder buildBufferEconomicEvent(HeadlineEconomicEvent& event);

// Utility functions

DateTimeType currentTime() noexcept;

} // namespace BlpConn

#endif // ECONOMIC_EVENT_H
