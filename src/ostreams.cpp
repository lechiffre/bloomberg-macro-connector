#include "blpconn_message.h"
#include <ctime>
#include <iomanip>
#include <ostream>

namespace BlpConn {

std::ostream& operator<<(std::ostream& os, const ValueType& value) {
    os << "ValueType { number: " << value.number
       << ", value: " << value.value
       << ", low: " << value.low
       << ", high: " << value.high
       << ", median: " << value.median
       << ", average: " << value.average
       << ", standard_deviation: " << value.standard_deviation
       << " }";
    return os;
}

std::ostream& operator<<(std::ostream& os, const HeadlineBaseEvent& event) {
    os << "HeadlineBaseEvent { id_bb_global: " << event.id_bb_global
       << ", parsekyable_des: " << event.parsekyable_des
       << ", description: " << event.description
       << ", event_type: " << eventTypeToString(event.event_type)
       << ", event_subtype: " << eventSubTypeToString(event.event_subtype)
       << ", event_id: " << event.event_id
       << ", observation_period: " << event.observation_period
       << ", release_start_dt: { microseconds: " << event.release_start_dt.microseconds
       << ", offset: " << event.release_start_dt.offset << " }"
       << ", release_end_dt: { microseconds: " << event.release_end_dt.microseconds
       << ", offset: " << event.release_end_dt.offset << " }"
       << " }";
    return os;
}

std::ostream& operator<<(std::ostream& os, const HeadlineEconomicEvent& event) {
    os << static_cast<const HeadlineBaseEvent&>(event)
       << ", value: " << event.value
       << ", prior_value: " << event.prior_value
       << ", prior_event_id: " << event.prior_event_id
       << ", prior_observation_period: " << event.prior_observation_period
       << ", prior_economic_release_start_dt: { microseconds: " << event.prior_economic_release_start_dt.microseconds
       << ", offset: " << event.prior_economic_release_start_dt.offset << " }"
       << ", prior_economic_release_end_dt: { microseconds: " << event.prior_economic_release_end_dt.microseconds
       << ", offset: " << event.prior_economic_release_end_dt.offset << " }"
       << " }";
    return os;
}

std::ostream& operator<<(std::ostream& os, const HeadlineCalendarEvent& event) {
    os << static_cast<const HeadlineBaseEvent&>(event)
       << ", release_status: " << releaseStatusToString(event.release_status)
       << " }";
    return os;
}

std::ostream& operator<<(std::ostream& os, const DateTimeType& dt) {
    // Convert to a human-readable format
    std::time_t time = dt.microseconds / 1000000; // Convert microseconds to seconds
    std::tm* tm = std::localtime(&time);
    os << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    return os;
}

std::ostream& operator<<(std::ostream& os, const LogMessage& log_message) {
    os << log_message.log_dt << " : "
       << log_message.module_name << " : "
       << log_message.message;
}

} // namespace BlpConn