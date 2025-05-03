#include <ctime>
#include <cmath>
#include <string>
#include <blpapi_datetime.h>
#include <blpapi_name.h>
#include <blpapi_element.h>
#include "economic_event.h"

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
static const blpapi::Name PRIOR_ECONOMIC_RELEASE_DT("PRIOR_ECONOMIC_RELEASE_DT");
static const blpapi::Name PRIOR_ECONOMIC_PERIOD("PRIOR_ECONOMIC_PERIOD");
static const blpapi::Name PRIOR_OBSERVATION_PERIOD("PRIOR_OBSERVATION_PERIOD");
static const blpapi::Name PRIOR_EVENT_ID("PRIOR_EVENT_ID");
static const blpapi::Name RELEASE_STATUS("RELEASE_STATUS");

struct DateTimeInterval {
    DateTimeType start;
    DateTimeType end;
};

DateTimeType
convertToDateTime(const blpapi::Datetime& blpDatetime) {
    // Validate the year
    if (blpDatetime.year() == 0) {
        throw std::invalid_argument("Year is not set in blpapi::Datetime.");
    }

    // Validate the month
    if (blpDatetime.month() < 1 || blpDatetime.month() > 12) {
        throw std::invalid_argument("Month is out of range in blpapi::Datetime.");
    }

    // Validate the day
    static const int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int maxDays = daysInMonth[blpDatetime.month() - 1];

    // Check for leap year in February
    if (blpDatetime.month() == 2 && ((blpDatetime.year() % 4 == 0 && blpDatetime.year() % 100 != 0) || (blpDatetime.year() % 400 == 0))) {
        maxDays = 29;
    }

    if (blpDatetime.day() < 1 || blpDatetime.day() > static_cast<unsigned int>(maxDays)) {
        throw std::invalid_argument("Day is out of range in blpapi::Datetime.");
    }

    DateTimeType dateTime;

    // Create a tm structure for conversion
    std::tm timeStruct = {};
    timeStruct.tm_year = blpDatetime.year() - 1900;
    timeStruct.tm_mon = blpDatetime.month() - 1;   // tm_mon is 0-based
    timeStruct.tm_mday = blpDatetime.day();
    timeStruct.tm_hour = blpDatetime.hours();
    timeStruct.tm_min = blpDatetime.minutes();
    timeStruct.tm_sec = blpDatetime.seconds();

    // Convert to time_t (seconds since Unix epoch)
    std::time_t timeSinceEpoch = std::mktime(&timeStruct);

    // Calculate microseconds since Unix epoch
    dateTime.microseconds = static_cast<uint64_t>(timeSinceEpoch) * 1000000 + blpDatetime.microseconds();

    // Set the offset
    dateTime.offset = static_cast<int16_t>(blpDatetime.offset());

    return dateTime;
}

struct DateTimeInterval
getReleaseDT(const blpapi::Element& elem, const blpapi::Name& name) {
    struct DateTimeInterval interval;
    if (!elem.hasElement(name)) {
        throw std::runtime_error("Element not found: " + std::string(name.string()));
    }

    const blpapi::Element subElem = elem.getElement(name);

    if (!subElem.isValid()) {
        throw std::runtime_error("Sub-element is invalid.");
    }

    const blpapi::Element choice = subElem.getChoice();

    if (!choice.isValid()) {
        throw std::runtime_error("Choice element is invalid.");
    }


    if (choice.name() == DATETIME || choice.name() == DATE) {
        if (choice.isNull()) {
            throw std::runtime_error("Choice element has no value.");
        }
        const blpapi::Datetime start = choice.getValueAsDatetime();
        interval.start = convertToDateTime(start);
        // TODO: eleminate duplicated execution
        interval.end = convertToDateTime(start);
        return interval;
    } else if (choice.name() == DATARANGE) {
        if (!choice.hasElement(START) || !choice.hasElement(END)) {
            throw std::runtime_error("DATARANGE element is missing START or END.");
        }
        const blpapi::Datetime start = choice.getElement(START).getValueAsDatetime();
        const blpapi::Datetime end = choice.getElement(END).getValueAsDatetime();
        interval.start = convertToDateTime(start);
        interval.end = convertToDateTime(end);
        return interval;
    }

    throw std::runtime_error("Unexpected choice type: " + std::string(choice.name().string()));
}


double getFloatFromStringElement(const blpapi::Element& elem) {
    std::string s = elem.getValueAsString();
    try {
        return stod(s);
    } catch (const std::invalid_argument& e) {
        return std::nan("");
    } catch (const std::out_of_range& e) {
        return std::nan("");
    }
}

double getFloatFromString(const blpapi::Element& elem, blpapi::Name name) {
    if (!elem.hasElement(name)) {
        return std::nan("");
    }
    return getFloatFromStringElement(elem.getElement(name));
}

int getIntFromString(const blpapi::Element& elem, const blpapi::Name name) {
    if (!elem.hasElement(name)) {
        return 0;
    }
    std::string s = elem.getElement(name).getValueAsString();
    return atoi(s.c_str());
}

ValueType getValue(const blpapi::Element& elem, const blpapi::Name name) {
    ValueType value;
    if (!elem.hasElement(name)) {
        return value;
    }


    blpapi::Element subElem = elem.getElement(name);
    blpapi::Element choice = subElem.getChoice();
    std::cout << ">>> Single: " << choice << std::endl;
    if ((choice.name() == SINGLE)) {
        value.number = 1;
        value.value = getFloatFromStringElement(choice);
    } else if ((choice.name() == DISTRIBUTION)) {
        value.number = getIntFromString(choice, NUMBER);
        value.average = getFloatFromString(choice, AVERAGE);
        value.low = getFloatFromString(choice, LOW);
        value.high = getFloatFromString(choice, HIGH);
        value.median = getFloatFromString(choice, MEDIAN);
        value.standard_deviation = getFloatFromString(choice, STANDARD_DEVIATION);
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
        case EventType::Actual: return "ACTUAL";
        case EventType::Revision: return "REVISION";
        case EventType::Estimate: return "ESTIMATE";
        case EventType::Calendar: return "CALENDAR";
        default: return "UNKNOWN";
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
        case EventSubType::New: return "NEW";
        case EventSubType::Update: return "UPDATE";
        case EventSubType::Unitpaint: return "INITPAINT";
        case EventSubType::Delete: return "DELETE";
        default: return "UNKNOWN";
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
        case ReleaseStatus::Released: return "RELEASED";
        case ReleaseStatus::Scheduled: return "SCHEDULED";
        default: return "UNKNOWN";
    }
}

void parseHeadlineBaseEvent(const blpapi::Element elem, HeadlineBaseEvent& message) {
    // Required fields
    message.id_bb_global = elem.getElement(ID_BB_GLOBAL).getValueAsString();
    message.parsekyable_des = elem.getElement(PARSEKYABLE_DES).getValueAsString();
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
        message.observation_period = elem.getElement(OBSERVATION_PERIOD).getValueAsString();
    }
    if (elem.hasElement(ECO_RELEASE_DT)) {
        DateTimeInterval interval = getReleaseDT(elem, ECO_RELEASE_DT);
        message.release_start_dt = interval.start;
        message.release_end_dt = interval.end;
    }
}

HeadlineEconomicEvent parseHeadlineEconomicEvent(const blpapi::Element& elem) {
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
            message.prior_event_id = subElem.getElement(PRIOR_EVENT_ID).getValueAsInt32();
        }
        if (subElem.hasElement(PRIOR_OBSERVATION_PERIOD)) {
            message.prior_observation_period = subElem.getElement(PRIOR_OBSERVATION_PERIOD)
                .getValueAsString();
        }
        if (subElem.hasElement(PRIOR_ECONOMIC_RELEASE_DT)) {
            DateTimeInterval interval = getReleaseDT(subElem, PRIOR_ECONOMIC_RELEASE_DT);
            message.prior_economic_release_start_dt = interval.start;
            message.prior_economic_release_end_dt = interval.end;
        }
    }
    return message;
}

HeadlineCalendarEvent parseHeadlineCalendarEvent(const blpapi::Element& elem) { 
    HeadlineCalendarEvent message;
    parseHeadlineBaseEvent(elem, message);
    if (elem.hasElement(RELEASE_STATUS)) {
        std::string s = elem.getElement(RELEASE_STATUS).getValueAsString();
        message.release_status = stringToReleaseStatus(s);
    }
    return message;
}

} // namespace BlpConn
