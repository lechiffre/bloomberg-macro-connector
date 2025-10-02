#ifndef _BLPCONN_DESERIALIZE_H_
#define _BLPCONN_DESERIALIZE_H_

#include "blpconn_fb_generated.h"
#include "blpconn_message.h"
#include "blpconn_profiler.h"

namespace BlpConn {

HeadlineCalendarEvent parseHeadlineCalendarEvent(const blpapi::Element &elem);
HeadlineEconomicEvent parseHeadlineEconomicEvent(const blpapi::Element &elem);
MacroReferenceData parseMacroReferenceData(const blpapi::Element &elem);
MacroHeadLineEvent parseMacroHeadlineEvent(const blpapi::Element &elem);
MacroCalendarEvent parseMacroCalendarEvent(const blpapi::Element &elem);

DateTimeType convertToDateTime(const blpapi::Datetime &dt);

EventType stringToEventType(const std::string &s);
std::string eventTypeToString(EventType t);

EventSubType stringToEventSubType(const std::string &s);
std::string eventSubTypeToString(EventSubType t);

ReleaseStatus stringToReleaseStatus(const std::string &s);
std::string releaseStatusToString(ReleaseStatus status);

std::ostream &operator<<(std::ostream &os, const DateTimeType &dt);
std::ostream &operator<<(std::ostream &os, const ValueType &value);
std::ostream &operator<<(std::ostream &os, const HeadlineBaseEvent &event);
std::ostream &operator<<(std::ostream &os, const HeadlineEconomicEvent &event);
std::ostream &operator<<(std::ostream &os, const HeadlineCalendarEvent &event);
std::ostream &operator<<(std::ostream &os, const MacroReferenceData &data);
std::ostream &operator<<(std::ostream &os, const MacroHeadLineEvent &event);
std::ostream &operator<<(std::ostream &os, const MacroCalendarEvent &event);
std::ostream &operator<<(std::ostream &os, const LogMessage &log_message);

HeadlineEconomicEvent
toHeadlineEconomicEvent(const FB::HeadlineEconomicEvent *fb_event);
HeadlineCalendarEvent
toHeadlineCalendarEvent(const FB::HeadlineCalendarEvent *fb_event);

LogMessage toLogMessage(const FB::LogMessage *fb_log_message);


MacroReferenceData toMacroReferenceData(
        const BlpConn::FB::MacroReferenceData* fb_data);
MacroHeadLineEvent toMacroHeadlineEvent(
        const BlpConn::FB::MacroHeadlineEvent* fb_event);
MacroCalendarEvent toMacroCalendarEvent(
        const BlpConn::FB::MacroCalendarEvent* fb_event);

flatbuffers::FlatBufferBuilder
buildBufferEconomicEvent(HeadlineEconomicEvent &event);

flatbuffers::FlatBufferBuilder
buildBufferEconomicEvent(const blpapi::Element &elem);

flatbuffers::FlatBufferBuilder
buildBufferCalendarEvent(HeadlineCalendarEvent &event);

flatbuffers::FlatBufferBuilder
buildBufferCalendarEvent(const blpapi::Element &elem);


flatbuffers::FlatBufferBuilder buildBufferMacroReferenceData(
        const blpapi::Element& elem);

flatbuffers::FlatBufferBuilder buildBufferMacroHeadlineEvent(
        const blpapi::Element& elem);

flatbuffers::FlatBufferBuilder buildBufferMacroCalendarEvent(
        const blpapi::Element& elem);

flatbuffers::FlatBufferBuilder buildBufferLogMessage(LogMessage &log_message);

// Utility functions
DateTimeType currentTime() noexcept;
std::string currentTimeStamp();

} // namespace BlpConn

#endif // _BLPCONN_DESERIALIZE_H_
