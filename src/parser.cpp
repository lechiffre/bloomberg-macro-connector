#include <blpapi_datetime.h>
#include <blpapi_element.h>
#include <blpapi_name.h>

#include <cmath>
#include <ctime>
#include <string>

#include "blpconn_fb_generated.h"
#include "blpconn_message.h"
#include "blpconn_profiler.h"

using namespace BloombergLP;

namespace BlpConn {

static const blpapi::Name DATETIME("DATETIME");
static const blpapi::Name DATE("DATE");
static const blpapi::Name DATARANGE("DATARANGE");
static const blpapi::Name START("START");
static const blpapi::Name END("END");

static const blpapi::Name SINGLE("SINGLE");
static const blpapi::Name NUMBER("NUMBER");
static const blpapi::Name DISTRIBUTION("DISTRIBUTION");
static const blpapi::Name AVERAGE("AVERAGE");
static const blpapi::Name LOW("LOW");
static const blpapi::Name HIGH("HIGH");
static const blpapi::Name MEDIAN("MEDIAN");
static const blpapi::Name STANDARD_DEVIATION("STANDARD_DEVIATION");

static const blpapi::Name ID_BB_GLOBAL("ID_BB_GLOBAL");
static const blpapi::Name PARSEKYABLE_DES("PARSEKYABLE_DES");
static const blpapi::Name DESCRIPTION("DESCRIPTION");
static const blpapi::Name EVENT_TYPE("EVENT_TYPE");
static const blpapi::Name EVENT_SUBTYPE("EVENT_SUBTYPE");
static const blpapi::Name EVENT_ID("EVENT_ID");
static const blpapi::Name OBSERVATION_PERIOD("OBSERVATION_PERIOD");
static const blpapi::Name ECO_RELEASE_DT("ECO_RELEASE_DT");
static const blpapi::Name VALUE("VALUE");
static const blpapi::Name PRIOR_VALUE("PRIOR_VALUE");
static const blpapi::Name REVISION_METADATA("REVISION_METADATA");
static const blpapi::Name PRIOR_ECONOMIC_RELEASE_DT("PRIOR_ECO_RELEASE_DT");
static const blpapi::Name PRIOR_OBSERVATION_PERIOD("PRIOR_OBSERVATION_PERIOD");
static const blpapi::Name PRIOR_EVENT_ID("PRIOR_EVENT_ID");
static const blpapi::Name RELEASE_STATUS("RELEASE_STATUS");

static const blpapi::Name INDX_FREQ("INDX_FREQ");
static const blpapi::Name INDX_UNITS("INDX_UNITS");
static const blpapi::Name COUNTRY_ISO("COUNTRY_ISO");
static const blpapi::Name INDX_SOURCE("INDX_SOURCE");
static const blpapi::Name SEASONALITY_TRANSFORMATION(
        "SEASONALITY_AND_TRANSFORMATION");
static const blpapi::Name RELEVANCE_VALUE("RELEVANCE_VALUE");

struct DateTimeInterval {
    DateTimeType start;
    DateTimeType end;
};

static std::string getString(const blpapi::Element& e, const blpapi::Name& n) {
    return e.hasElement(n) ? e.getElement(n).getValueAsString() : "";
}

static int32_t getInt32(const blpapi::Element& e, const blpapi::Name& n) {
    return e.hasElement(n) ? e.getElement(n).getValueAsInt32() : 0;
}

static int getIntFromStringElement(const blpapi::Element& e) {
    std::string s = e.getValueAsString();
    try {
        return std::stoi(s);
    } catch (...) {
        return 0;
    }
}

static int getIntFromString(const blpapi::Element& elem,
                            const blpapi::Name name) {
    if (!elem.hasElement(name)) {
        return 0;
    }
    /*
    std::string s = elem.getElement(name).getValueAsString();
    return atoi(s.c_str());
    */
    return getIntFromStringElement(elem.getElement(name));
}

static double getFloatFromStringElement(const blpapi::Element& e) {
    std::string s = e.getValueAsString();
    try {
        return std::stod(s);
    } catch (...) {
        return std::nan("");
    }
}

static double getFloatFromString(const blpapi::Element& elem,
                                 blpapi::Name name) {
    if (!elem.hasElement(name)) {
        return std::nan("");
    }
    return getFloatFromStringElement(elem.getElement(name));
}

static uint64_t toMicrosecondsSinceEpoch(const blpapi::Datetime& blpDatetime) {
    std::tm timeStruct = {};
    if (blpDatetime.hasParts(blpapi::DatetimeParts::DATE)) {
        timeStruct.tm_year = blpDatetime.year() - 1900;
        timeStruct.tm_mon = blpDatetime.month() - 1;
        timeStruct.tm_mday = blpDatetime.day();
    }
    if (blpDatetime.hasParts(blpapi::DatetimeParts::TIME)) {
        timeStruct.tm_hour = blpDatetime.hours();
        timeStruct.tm_min = blpDatetime.minutes();
        timeStruct.tm_sec = blpDatetime.seconds();
    }
    std::time_t timeSinceEpoch = timegm(&timeStruct);
    int64_t micros = blpDatetime.hasParts(blpapi::DatetimeParts::FRACSECONDS)
                         ? blpDatetime.microseconds()
                         : 0;
    return static_cast<uint64_t>(timeSinceEpoch) * 1000000 + micros;
}

DateTimeType convertToDateTime(const blpapi::Datetime& blpDatetime) {
    DateTimeType dateTime;
    dateTime.microseconds = toMicrosecondsSinceEpoch(blpDatetime);
    dateTime.offset = static_cast<int16_t>(blpDatetime.offset());
    return dateTime;
}

static struct DateTimeInterval getReleaseDT(const blpapi::Element& elem,
                                            const blpapi::Name& name) {
    struct DateTimeInterval interval;
    interval.start = convertToDateTime(blpapi::Datetime());
    interval.end = convertToDateTime(blpapi::Datetime());
    // Use default values if element or subelements are missing/invalid
    if (!elem.hasElement(name)) {
        return interval;
    }

    const blpapi::Element subElem = elem.getElement(name);
    if (!subElem.isValid()) {
        return interval;
    }

    const blpapi::Element choice = subElem.getChoice();
    if (!choice.isValid()) {
        return interval;
    }

    if (choice.name() == DATETIME || choice.name() == DATE) {
        if (choice.isNull()) {
            return interval;
        }
        const blpapi::Datetime start = choice.getValueAsDatetime();
        interval.start = convertToDateTime(start);
        interval.end = convertToDateTime(start);
        return interval;
    } else if (choice.name() == DATARANGE) {
        if (!choice.hasElement(START) || !choice.hasElement(END)) {
            return interval;
        }
        const blpapi::Datetime start =
            choice.getElement(START).getValueAsDatetime();
        const blpapi::Datetime end =
            choice.getElement(END).getValueAsDatetime();
        interval.start = convertToDateTime(start);
        interval.end = convertToDateTime(end);
        return interval;
    }

    // Unexpected type, use default values
    return interval;
}

ValueType getValue(const blpapi::Element& elem, const blpapi::Name name) {
    ValueType value;
    if (!elem.hasElement(name)) {
        return value;
    }

    blpapi::Element subElem = elem.getElement(name);
    blpapi::Element choice = subElem.getChoice();
    if ((choice.name() == SINGLE)) {
        value.number = 1;
        value.value = getFloatFromStringElement(choice);
    } else if ((choice.name() == DISTRIBUTION)) {
        value.number = getIntFromString(choice, NUMBER);
        value.average = getFloatFromString(choice, AVERAGE);
        value.low = getFloatFromString(choice, LOW);
        value.high = getFloatFromString(choice, HIGH);
        value.median = getFloatFromString(choice, MEDIAN);
        value.standard_deviation =
            getFloatFromString(choice, STANDARD_DEVIATION);
    }
    return value;
}

EventType stringToEventType(const std::string& s) {
    EventType t = EventType::Unknown;
    if (s == "ACTUAL") {
        t = EventType::Actual;
    } else if (s == "REVISION") {
        t = EventType::Revision;
    } else if (s == "ESTIMATE") {
        t = EventType::Estimate;
    } else if (s == "CALENDAR") {
        t = EventType::Calendar;
    }
    return t;
}

std::string eventTypeToString(EventType t) {
    switch (t) {
        case EventType::Actual:
            return "ACTUAL";
        case EventType::Revision:
            return "REVISION";
        case EventType::Estimate:
            return "ESTIMATE";
        case EventType::Calendar:
            return "CALENDAR";
        default:
            return "UNKNOWN";
    }
}

EventSubType stringToEventSubType(const std::string& s) {
    EventSubType t = EventSubType::Unknown;
    if (s == "NEW") {
        t = EventSubType::New;
    } else if (s == "UPDATE") {
        t = EventSubType::Update;
    } else if (s == "INITPAINT") {
        t = EventSubType::Unitpaint;
    } else if (s == "DELETE") {
        t = EventSubType::Delete;
    }
    return t;
}

std::string eventSubTypeToString(EventSubType t) {
    switch (t) {
        case EventSubType::New:
            return "NEW";
        case EventSubType::Update:
            return "UPDATE";
        case EventSubType::Unitpaint:
            return "INITPAINT";
        case EventSubType::Delete:
            return "DELETE";
        default:
            return "UNKNOWN";
    }
}

ReleaseStatus stringToReleaseStatus(const std::string& s) {
    ReleaseStatus status = ReleaseStatus::Unknown;
    if (s == "RELEASED") {
        status = ReleaseStatus::Released;
    } else if (s == "SCHEDULED") {
        status = ReleaseStatus::Scheduled;
    }
    return status;
}

std::string releaseStatusToString(ReleaseStatus status) {
    switch (status) {
        case ReleaseStatus::Released:
            return "RELEASED";
        case ReleaseStatus::Scheduled:
            return "SCHEDULED";
        default:
            return "UNKNOWN";
    }
}

static int eventTypeFromString(const std::string& s) {
    if (s == "ACTUAL") return static_cast<int>(FB::EventType::EventType_Actual);
    if (s == "REVISION")
        return static_cast<int>(FB::EventType::EventType_Revision);
    if (s == "ESTIMATE")
        return static_cast<int>(FB::EventType::EventType_Estimate);
    if (s == "CALENDAR")
        return static_cast<int>(FB::EventType::EventType_Calendar);
    return 0;
}

static int eventSubTypeFromString(const std::string& s) {
    if (s == "NEW") return static_cast<int>(FB::EventSubType::EventSubType_New);
    if (s == "UPDATE")
        return static_cast<int>(FB::EventSubType::EventSubType_Update);
    if (s == "INITPAINT")
        return static_cast<int>(FB::EventSubType::EventSubType_Unitpaint);
    if (s == "DELETE")
        return static_cast<int>(FB::EventSubType::EventSubType_Delete);
    return 0;
}

static int releaseStatusFromString(const std::string& s) {
    if (s == "RELEASED")
        return static_cast<int>(FB::ReleaseStatus::ReleaseStatus_Released);
    if (s == "SCHEDULED")
        return static_cast<int>(FB::ReleaseStatus::ReleaseStatus_Scheduled);
    return 0;
}

static void toFlatBuffersDateTimeFields(const blpapi::Datetime& blpDatetime,
                                        uint64_t& microseconds,
                                        int16_t& offset) {
    microseconds = toMicrosecondsSinceEpoch(blpDatetime);
    offset = static_cast<int16_t>(blpDatetime.offset());
}

flatbuffers::Offset<FB::DateTime> serializeDatetime(
    flatbuffers::FlatBufferBuilder& builder,
    const blpapi::Datetime& blpDatetime) {
    uint64_t microseconds;
    int16_t offset;
    toFlatBuffersDateTimeFields(blpDatetime, microseconds, offset);
    return FB::CreateDateTime(builder, microseconds, offset);
}

flatbuffers::Offset<FB::Value> serializeValueType(
    flatbuffers::FlatBufferBuilder& builder, const ValueType& value) {
    return FB::CreateValue(builder, value.number, value.value, value.low,
                           value.high, value.median, value.average,
                           value.standard_deviation);
}

flatbuffers::Offset<FB::Value> serializeValue(
    flatbuffers::FlatBufferBuilder& builder, const blpapi::Element& elem,
    const blpapi::Name name) {
    if (!elem.hasElement(name)) {
        return FB::CreateValue(builder, 0, std::nan(""), std::nan(""),
                               std::nan(""), std::nan(""), std::nan(""),
                               std::nan(""));
    }
    blpapi::Element subElem = elem.getElement(name);
    blpapi::Element choice = subElem.getChoice();
    if (choice.name() == SINGLE) {
        double value = getFloatFromStringElement(choice);
        return FB::CreateValue(builder, 1, value, std::nan(""), std::nan(""),
                               std::nan(""), std::nan(""), std::nan(""));
    } else if (choice.name() == DISTRIBUTION) {
        int number = choice.hasElement(NUMBER)
                         ? getIntFromStringElement(choice.getElement(NUMBER))
                         : 0;
        double average =
            choice.hasElement(AVERAGE)
                ? getFloatFromStringElement(choice.getElement(AVERAGE))
                : std::nan("");
        double low = choice.hasElement(LOW)
                         ? getFloatFromStringElement(choice.getElement(LOW))
                         : std::nan("");
        double high = choice.hasElement(HIGH)
                          ? getFloatFromStringElement(choice.getElement(HIGH))
                          : std::nan("");
        double median =
            choice.hasElement(MEDIAN)
                ? getFloatFromStringElement(choice.getElement(MEDIAN))
                : std::nan("");
        double stddev = choice.hasElement(STANDARD_DEVIATION)
                            ? getFloatFromStringElement(
                                  choice.getElement(STANDARD_DEVIATION))
                            : std::nan("");
        return FB::CreateValue(builder, number, std::nan(""), low, high, median,
                               average, stddev);
    }
    return FB::CreateValue(builder, 0, std::nan(""), std::nan(""), std::nan(""),
                           std::nan(""), std::nan(""), std::nan(""));
}

void extractReleaseDateTimes(flatbuffers::FlatBufferBuilder& builder,
                             const blpapi::Element& elem,
                             const blpapi::Name& name,
                             flatbuffers::Offset<FB::DateTime>& start,
                             flatbuffers::Offset<FB::DateTime>& end) {
    if (!elem.hasElement(name)) {
        start = serializeDatetime(builder, blpapi::Datetime());
        end = serializeDatetime(builder, blpapi::Datetime());
        return;
    }
    blpapi::Element dtElem = elem.getElement(name);
    blpapi::Element choice = dtElem.getChoice();
    if (choice.name() == DATETIME || choice.name() == DATE) {
        blpapi::Datetime dt = choice.getValueAsDatetime();
        start = serializeDatetime(builder, dt);
        end = serializeDatetime(builder, dt);
    } else if (choice.name() == DATARANGE) {
        blpapi::Datetime s = choice.getElement(START).getValueAsDatetime();
        blpapi::Datetime e = choice.getElement(END).getValueAsDatetime();
        start = serializeDatetime(builder, s);
        end = serializeDatetime(builder, e);
    } else {
        start = serializeDatetime(builder, blpapi::Datetime());
        end = serializeDatetime(builder, blpapi::Datetime());
    }
}

static flatbuffers::Offset<flatbuffers::String> fbString(
    flatbuffers::FlatBufferBuilder& builder, const blpapi::Element& elem,
    const blpapi::Name& name) {
    return builder.CreateString(getString(elem, name));
}

flatbuffers::Offset<FB::HeadlineEconomicEvent> serializeHeadlineEconomicEvent(
    flatbuffers::FlatBufferBuilder& builder, const blpapi::Element& elem) {
    auto id_bb_global = fbString(builder, elem, ID_BB_GLOBAL);
    auto parsekyable_des = fbString(builder, elem, PARSEKYABLE_DES);
    auto description = fbString(builder, elem, DESCRIPTION);
    auto observation_period = fbString(builder, elem, OBSERVATION_PERIOD);
    int event_type = elem.hasElement(EVENT_TYPE)
                         ? eventTypeFromString(getString(elem, EVENT_TYPE))
                         : 0;
    int event_subtype =
        elem.hasElement(EVENT_SUBTYPE)
            ? eventSubTypeFromString(getString(elem, EVENT_SUBTYPE))
            : 0;
    int32_t event_id = getInt32(elem, EVENT_ID);

    flatbuffers::Offset<FB::DateTime> release_start_dt, release_end_dt;
    extractReleaseDateTimes(builder, elem, ECO_RELEASE_DT, release_start_dt,
                            release_end_dt);

    auto value = serializeValue(builder, elem, VALUE);
    auto prior_value = serializeValue(builder, elem, PRIOR_VALUE);

    int32_t prior_event_id = 0;
    auto prior_observation_period = builder.CreateString("");
    // Assign default values for prior_release_start_dt and prior_release_end_dt
    flatbuffers::Offset<FB::DateTime> prior_release_start_dt =
        serializeDatetime(builder, blpapi::Datetime());
    flatbuffers::Offset<FB::DateTime> prior_release_end_dt =
        serializeDatetime(builder, blpapi::Datetime());
    if (elem.hasElement(REVISION_METADATA)) {
        blpapi::Element revElem = elem.getElement(REVISION_METADATA);
        if (revElem.hasElement(PRIOR_EVENT_ID)) {
            prior_event_id = getInt32(revElem, PRIOR_EVENT_ID);
        }
        if (revElem.hasElement(PRIOR_OBSERVATION_PERIOD)) {
            prior_observation_period =
                fbString(builder, revElem, PRIOR_OBSERVATION_PERIOD);
        }
        if (revElem.hasElement(PRIOR_ECONOMIC_RELEASE_DT)) {
            extractReleaseDateTimes(builder, revElem, PRIOR_ECONOMIC_RELEASE_DT,
                                    prior_release_start_dt,
                                    prior_release_end_dt);
        }
    }
    return FB::CreateHeadlineEconomicEvent(
        builder, id_bb_global, parsekyable_des, description,
        static_cast<FB::EventType>(event_type),
        static_cast<FB::EventSubType>(event_subtype), event_id,
        observation_period, release_start_dt, release_end_dt, value,
        prior_value, prior_event_id, prior_observation_period,
        prior_release_start_dt, prior_release_end_dt);
}

flatbuffers::Offset<FB::HeadlineCalendarEvent> serializeHeadlineCalendarEvent(
    flatbuffers::FlatBufferBuilder& builder, const blpapi::Element& elem) {
    auto id_bb_global = fbString(builder, elem, ID_BB_GLOBAL);
    auto parsekyable_des = fbString(builder, elem, PARSEKYABLE_DES);
    auto description = fbString(builder, elem, DESCRIPTION);
    auto observation_period = fbString(builder, elem, OBSERVATION_PERIOD);
    int event_type = elem.hasElement(EVENT_TYPE)
                         ? eventTypeFromString(getString(elem, EVENT_TYPE))
                         : 0;
    int event_subtype =
        elem.hasElement(EVENT_SUBTYPE)
            ? eventSubTypeFromString(getString(elem, EVENT_SUBTYPE))
            : 0;
    int32_t event_id = getInt32(elem, EVENT_ID);

    flatbuffers::Offset<FB::DateTime> release_start_dt, release_end_dt;
    extractReleaseDateTimes(builder, elem, ECO_RELEASE_DT, release_start_dt,
                            release_end_dt);

    int release_status =
        elem.hasElement(RELEASE_STATUS)
            ? releaseStatusFromString(getString(elem, RELEASE_STATUS))
            : 0;

    return FB::CreateHeadlineCalendarEvent(
        builder, id_bb_global, parsekyable_des, description,
        static_cast<FB::EventType>(event_type),
        static_cast<FB::EventSubType>(event_subtype), event_id,
        observation_period, release_start_dt, release_end_dt,
        static_cast<FB::ReleaseStatus>(release_status));
}

flatbuffers::Offset<FB::MacroReferenceData> serializeMacroReferenceData(
    flatbuffers::FlatBufferBuilder& builder, const blpapi::Element& elem) {
    auto id_bb_global = fbString(builder, elem, ID_BB_GLOBAL);
    auto parsekyable_des = fbString(builder, elem, PARSEKYABLE_DES);
    auto description = fbString(builder, elem, DESCRIPTION);
    auto indx_freq = fbString(builder, elem, INDX_FREQ);
    auto indx_units = fbString(builder, elem, INDX_UNITS);
    auto country_iso = fbString(builder, elem, COUNTRY_ISO);
    auto indx_source = fbString(builder, elem, INDX_SOURCE);
    auto seasonality_transformation =
        fbString(builder, elem, SEASONALITY_TRANSFORMATION);
    return FB::CreateMacroReferenceData(
        builder, id_bb_global, parsekyable_des, description, indx_freq,
        indx_units, country_iso, indx_source, seasonality_transformation);
}

flatbuffers::Offset<FB::MacroHeadlineEvent> serializeMacroHeadlineEvent(
    flatbuffers::FlatBufferBuilder& builder, const blpapi::Element& elem) {
    int event_type = elem.hasElement(EVENT_TYPE)
                         ? eventTypeFromString(getString(elem, EVENT_TYPE))
                         : 0;
    int event_subtype =
        elem.hasElement(EVENT_SUBTYPE)
            ? eventSubTypeFromString(getString(elem, EVENT_SUBTYPE))
            : 0;
    int32_t event_id = getInt32(elem, EVENT_ID);
    auto observation_period = fbString(builder, elem, OBSERVATION_PERIOD);

    flatbuffers::Offset<FB::DateTime> release_start_dt, release_end_dt;
    extractReleaseDateTimes(builder, elem, ECO_RELEASE_DT, release_start_dt,
                            release_end_dt);

    int32_t prior_event_id = 0;
    auto prior_observation_period = builder.CreateString("");
    flatbuffers::Offset<FB::DateTime> prior_release_start_dt =
        serializeDatetime(builder, blpapi::Datetime());
    flatbuffers::Offset<FB::DateTime> prior_release_end_dt =
        serializeDatetime(builder, blpapi::Datetime());
    if (elem.hasElement(REVISION_METADATA)) {
        blpapi::Element revElem = elem.getElement(REVISION_METADATA);
        if (revElem.hasElement(PRIOR_EVENT_ID)) {
            prior_event_id = getInt32(revElem, PRIOR_EVENT_ID);
        }
        if (revElem.hasElement(PRIOR_OBSERVATION_PERIOD)) {
            prior_observation_period =
                fbString(builder, revElem, PRIOR_OBSERVATION_PERIOD);
        }
        if (revElem.hasElement(PRIOR_ECONOMIC_RELEASE_DT)) {
            extractReleaseDateTimes(builder, revElem,
                                    PRIOR_ECONOMIC_RELEASE_DT,
                                    prior_release_start_dt,
                                    prior_release_end_dt);
        }
    }
    auto value = serializeValue(builder, elem, VALUE);
    return FB::CreateMacroHeadlineEvent(
        builder, static_cast<FB::EventType>(event_type),
        static_cast<FB::EventSubType>(event_subtype), event_id,
        observation_period, release_start_dt, release_end_dt, prior_event_id,
        prior_observation_period, prior_release_start_dt,
        prior_release_end_dt, value);
}

flatbuffers::Offset<FB::MacroCalendarEvent> serializeMacroCalendarEvent(
    flatbuffers::FlatBufferBuilder& builder, const blpapi::Element& elem) {
    auto id_bb_global = fbString(builder, elem, ID_BB_GLOBAL);
    auto parsekyable_des = fbString(builder, elem, PARSEKYABLE_DES);
    int event_type = elem.hasElement(EVENT_TYPE)
                         ? eventTypeFromString(getString(elem, EVENT_TYPE))
                         : 0;
    int event_subtype =
        elem.hasElement(EVENT_SUBTYPE)
            ? eventSubTypeFromString(getString(elem, EVENT_SUBTYPE))
            : 0;
    auto description = fbString(builder, elem, DESCRIPTION);
    int32_t event_id = getInt32(elem, EVENT_ID);
    auto observation_period = fbString(builder, elem, OBSERVATION_PERIOD);
    flatbuffers::Offset<FB::DateTime> release_start_dt, release_end_dt;
    extractReleaseDateTimes(builder, elem, ECO_RELEASE_DT, release_start_dt,
                            release_end_dt);
    int release_status =
        elem.hasElement(RELEASE_STATUS)
            ? releaseStatusFromString(getString(elem, RELEASE_STATUS))
            : 0;

    double relevance_value = getFloatFromString(elem, RELEVANCE_VALUE);

    return FB::CreateMacroCalendarEvent(
        builder, id_bb_global, parsekyable_des,
        static_cast<FB::EventType>(event_type),
        static_cast<FB::EventSubType>(event_subtype),
        description,
        event_id,
        observation_period, release_start_dt, release_end_dt,
        static_cast<FB::ReleaseStatus>(release_status), relevance_value);
}

flatbuffers::FlatBufferBuilder buildBufferEconomicEvent(
    const blpapi::Element& elem) {
    flatbuffers::FlatBufferBuilder builder;
    auto fb_event = serializeHeadlineEconomicEvent(builder, elem).Union();
    auto fb_main = FB::CreateMain(
        builder, FB::Message::Message_HeadlineEconomicEvent, fb_event);
    builder.Finish(fb_main);
    return builder;
}

flatbuffers::FlatBufferBuilder buildBufferCalendarEvent(
    const blpapi::Element& elem) {
    flatbuffers::FlatBufferBuilder builder;
    auto fb_event = serializeHeadlineCalendarEvent(builder, elem).Union();
    auto fb_main = FB::CreateMain(
        builder, FB::Message::Message_HeadlineCalendarEvent, fb_event);
    builder.Finish(fb_main);
    return builder;
}

flatbuffers::FlatBufferBuilder buildBufferMacroReferenceData(
        const blpapi::Element& elem) {
    flatbuffers::FlatBufferBuilder builder;
    auto fb_macro_reference = serializeMacroReferenceData(builder, elem).Union();
    auto fb_main = FB::CreateMain(
        builder, FB::Message::Message_MacroReferenceData, fb_macro_reference);
    builder.Finish(fb_main);
    return builder;
}

flatbuffers::FlatBufferBuilder buildBufferMacroHeadlineEvent(
        const blpapi::Element& elem) {
    flatbuffers::FlatBufferBuilder builder;
    auto fb_macro_headline = serializeMacroHeadlineEvent(builder, elem).Union();
    auto fb_main = FB::CreateMain(
        builder, FB::Message::Message_MacroHeadlineEvent, fb_macro_headline);
    builder.Finish(fb_main);
    return builder;
}

flatbuffers::FlatBufferBuilder buildBufferMacroCalendarEvent(
        const blpapi::Element& elem) {
    flatbuffers::FlatBufferBuilder builder;
    auto fb_macro_calendar = serializeMacroCalendarEvent(builder, elem).Union();
    auto fb_main = FB::CreateMain(
        builder, FB::Message::Message_MacroCalendarEvent, fb_macro_calendar);
    builder.Finish(fb_main);
    return builder;
}

void parseHeadlineBaseEvent(const blpapi::Element elem,
                            HeadlineBaseEvent& message) {
    PROFILE_FUNCTION()
    // Required fields
    message.id_bb_global = elem.getElement(ID_BB_GLOBAL).getValueAsString();
    message.parsekyable_des =
        elem.getElement(PARSEKYABLE_DES).getValueAsString();
    // Optional fields
    if (elem.hasElement(DESCRIPTION)) {
        message.description = elem.getElement(DESCRIPTION).getValueAsString();
    }
    if (elem.hasElement(EVENT_TYPE)) {
        std::string s = elem.getElement(EVENT_TYPE).getValueAsString();
        message.event_type = stringToEventType(s);
    }
    if (elem.hasElement(EVENT_SUBTYPE)) {
        std::string s = elem.getElement(EVENT_SUBTYPE).getValueAsString();
        message.event_subtype = stringToEventSubType(s);
    }
    if (elem.hasElement(EVENT_ID)) {
        message.event_id = elem.getElement(EVENT_ID).getValueAsInt32();
    }
    if (elem.hasElement(OBSERVATION_PERIOD)) {
        message.observation_period =
            elem.getElement(OBSERVATION_PERIOD).getValueAsString();
    }
    if (elem.hasElement(ECO_RELEASE_DT)) {
        DateTimeInterval interval = getReleaseDT(elem, ECO_RELEASE_DT);
        message.release_start_dt = interval.start;
        message.release_end_dt = interval.end;
    }
    END_PROFILE_FUNCTION()
}

HeadlineEconomicEvent parseHeadlineEconomicEvent(const blpapi::Element& elem) {

    PROFILE_FUNCTION()
    HeadlineEconomicEvent message;
    parseHeadlineBaseEvent(elem, message);
    if (elem.hasElement(VALUE)) {
        message.value = getValue(elem, VALUE);
    }

    if (elem.hasElement(PRIOR_VALUE)) {
        message.prior_value = getValue(elem, PRIOR_VALUE);
    }

    if (elem.hasElement(REVISION_METADATA)) {
        blpapi::Element subElem = elem.getElement(REVISION_METADATA);
        if (subElem.hasElement(PRIOR_EVENT_ID)) {
            message.prior_event_id =
                subElem.getElement(PRIOR_EVENT_ID).getValueAsInt32();
        }
        if (subElem.hasElement(PRIOR_OBSERVATION_PERIOD)) {
            message.prior_observation_period =
                subElem.getElement(PRIOR_OBSERVATION_PERIOD).getValueAsString();
        }
        if (subElem.hasElement(PRIOR_ECONOMIC_RELEASE_DT)) {
            DateTimeInterval interval =
                getReleaseDT(subElem, PRIOR_ECONOMIC_RELEASE_DT);
            message.prior_economic_release_start_dt = interval.start;
            message.prior_economic_release_end_dt = interval.end;
        }
    }
    END_PROFILE_FUNCTION()
    return message;
}

HeadlineCalendarEvent parseHeadlineCalendarEvent(const blpapi::Element& elem) {
    PROFILE_FUNCTION()
    HeadlineCalendarEvent message;
    parseHeadlineBaseEvent(elem, message);

    if (elem.hasElement(RELEASE_STATUS)) {
        std::string s = elem.getElement(RELEASE_STATUS).getValueAsString();
        message.release_status = stringToReleaseStatus(s);
    }
    END_PROFILE_FUNCTION()
    return message;
}

MacroReferenceData parseMacroReferenceData(const blpapi::Element& elem) {
    PROFILE_FUNCTION()
    MacroReferenceData message;
    // Required fields
    message.id_bb_global = elem.getElement(ID_BB_GLOBAL).getValueAsString();
    message.parsekyable_des =
        elem.getElement(PARSEKYABLE_DES).getValueAsString();
    // Optional fields
    if (elem.hasElement(DESCRIPTION)) {
        message.description = elem.getElement(DESCRIPTION).getValueAsString();
    }
    if (elem.hasElement(INDX_FREQ)) {
        message.indx_freq = elem.getElement(INDX_FREQ).getValueAsString();
    }
    if (elem.hasElement(INDX_UNITS)) {
        message.indx_units = elem.getElement(INDX_UNITS).getValueAsString();
    }
    if (elem.hasElement(COUNTRY_ISO)) {
        message.country_iso = elem.getElement(COUNTRY_ISO).getValueAsString();
    }
    if (elem.hasElement(INDX_SOURCE)) {
        message.indx_source = elem.getElement(INDX_SOURCE).getValueAsString();
    }
    if (elem.hasElement(SEASONALITY_TRANSFORMATION)) {
        message.seasonality_transformation =
            elem.getElement(SEASONALITY_TRANSFORMATION).getValueAsString();
    }
    END_PROFILE_FUNCTION()
    return message;
}

MacroHeadLineEvent parseMacroHeadlineEvent(const blpapi::Element& elem) {
    PROFILE_FUNCTION()
    MacroHeadLineEvent message;
    if (elem.hasElement(EVENT_TYPE)) {
        std::string s = elem.getElement(EVENT_TYPE).getValueAsString();
        message.event_type = stringToEventType(s);
    }
    if (elem.hasElement(EVENT_SUBTYPE)) {
        std::string s = elem.getElement(EVENT_SUBTYPE).getValueAsString();
        message.event_subtype = stringToEventSubType(s);
    }
    if (elem.hasElement(EVENT_ID)) {
        message.event_id = elem.getElement(EVENT_ID).getValueAsInt32();
    }
    if (elem.hasElement(OBSERVATION_PERIOD)) {
        message.observation_period =
            elem.getElement(OBSERVATION_PERIOD).getValueAsString();
    }
    if (elem.hasElement(ECO_RELEASE_DT)) {
        DateTimeInterval interval = getReleaseDT(elem, ECO_RELEASE_DT);
        message.release_start_dt = interval.start;
        message.release_end_dt = interval.end;
    }
    if (elem.hasElement(PRIOR_EVENT_ID)) {
        message.prior_event_id = elem.getElement(PRIOR_EVENT_ID).getValueAsInt32();
    }
    if (elem.hasElement(PRIOR_OBSERVATION_PERIOD)) {
        message.prior_observation_period =
            elem.getElement(PRIOR_OBSERVATION_PERIOD).getValueAsString();
    }
    if (elem.hasElement(PRIOR_ECONOMIC_RELEASE_DT)) {
        DateTimeInterval interval =
            getReleaseDT(elem, PRIOR_ECONOMIC_RELEASE_DT);
        message.prior_economic_release_start_dt = interval.start;
        message.prior_economic_release_end_dt = interval.end;
    }
    if (elem.hasElement(VALUE)) {
        message.value = getValue(elem, VALUE);
    }
    END_PROFILE_FUNCTION()
    return message;
}

MacroCalendarEvent parseMacroCalendarEvent(const blpapi::Element& elem) {
    PROFILE_FUNCTION()
    MacroCalendarEvent message;
    // Required fields
    message.id_bb_global = elem.getElement(ID_BB_GLOBAL).getValueAsString();
    message.parsekyable_des =
        elem.getElement(PARSEKYABLE_DES).getValueAsString();
    // Optional fields
    if (elem.hasElement(EVENT_TYPE)) {
        std::string s = elem.getElement(EVENT_TYPE).getValueAsString();
        message.event_type = stringToEventType(s);
    }
    if (elem.hasElement(EVENT_SUBTYPE)) {
        std::string s = elem.getElement(EVENT_SUBTYPE).getValueAsString();
        message.event_subtype = stringToEventSubType(s);
    }
    if (elem.hasElement(DESCRIPTION)) {
        message.description = elem.getElement(DESCRIPTION).getValueAsString();
    }
    if (elem.hasElement(EVENT_ID)) {
        message.event_id = elem.getElement(EVENT_ID).getValueAsInt32();
    }
    if (elem.hasElement(OBSERVATION_PERIOD)) {
        message.observation_period =
            elem.getElement(OBSERVATION_PERIOD).getValueAsString();
    }
    if (elem.hasElement(ECO_RELEASE_DT)) {
        DateTimeInterval interval = getReleaseDT(elem, ECO_RELEASE_DT);
        message.release_start_dt = interval.start;
        message.release_end_dt = interval.end;
    }
    if (elem.hasElement(RELEASE_STATUS)) {
        std::string s = elem.getElement(RELEASE_STATUS).getValueAsString();
        message.release_status = stringToReleaseStatus(s);
    }
    if (elem.hasElement(RELEVANCE_VALUE)) {
        message.relevance_value =
            getFloatFromStringElement(elem.getElement(RELEVANCE_VALUE));
    }
    END_PROFILE_FUNCTION()
    return message;
}

}  // namespace BlpConn
