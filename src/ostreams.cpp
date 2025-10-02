// #include "blpconn_message.h"
#include <ctime>
#include <iomanip>
#include <ostream>
#include "blpconn_message.h"
#include "blpconn_deserialize.h"

namespace BlpConn {

std::string toString(const Module module) {
    switch (module) {
        case Module::System:
            return "System";
        case Module::Session:
            return "Session";
        case Module::Subscription:
            return "Subscription";
        case Module::Service:
            return "Service";
        case Module::Heartbeat:
            return "Heartbeat";
        case Module::Another:
            return "Another";
        default:
            return "Unknown";
    }
}

std::string toString(const SessionStatus status) {
    switch (status) {
        case SessionStatus::ConnectionUp:
            return "ConnectionUp";
        case SessionStatus::Started:
            return "Started";
        case SessionStatus::ConnectionDown:
            return "ConnectionDown";
        case SessionStatus::Terminated:
            return "Terminated";
        case SessionStatus::Another:
            return "Another";
        default:
            return "Unknown";
    }
}

std::string toString(const SubscriptionStatus status) {
    switch (status) {
        case SubscriptionStatus::Started:
            return "Started";
        case SubscriptionStatus::StreamsActivated:
            return "StreamsActivated";
        case SubscriptionStatus::Terminated:
            return "Terminated";
        case SubscriptionStatus::Failure:
            return "Failure";
        case SubscriptionStatus::Success:
            return "Success";
        case SubscriptionStatus::Another:
            return "Another";
        default:
            return "Unknown";
    }
}

std::string toString(const ServiceStatus status) {
    switch (status) {
        case ServiceStatus::Opened:
            return "Opened";
        case ServiceStatus::Closed:
            return "Closed";
        case ServiceStatus::Failure:
            return "Failure";
        case ServiceStatus::Another:
            return "Another";
        default:
            return "Unknown";
    }
}

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
    Module module = static_cast<Module>(log_message.module);
    os << log_message.log_dt << "|"
       << toString(module) << "|";

    switch (module) {
        case Module::Session: {
            const SessionStatus status = static_cast<SessionStatus>(log_message.status);
            os << toString(status);
            break;
        }
        case Module::Service: {
            const ServiceStatus status = static_cast<ServiceStatus>(log_message.status);
            os << toString(status);
            break;
        }
        case Module::Subscription: {
            const SubscriptionStatus status = static_cast<SubscriptionStatus>(log_message.status);
            os << toString(status);
            break;
        }
        default:
            os << "Unknown";
            break;
    }

    os << "|CorrelationID(" << log_message.correlation_id << ")"
       << "|" << log_message.message;
    return os;
}

std::ostream& operator<<(std::ostream& os, const MacroReferenceData& data) {
    os << "MacroReferenceData { id_bb_global: " << data.id_bb_global
         << ", parsekyable_des: " << data.parsekyable_des
            << ", description: " << data.description
            << ", indx_freq: " << data.indx_freq
            << ", indx_units: " << data.indx_units
            << ", country_iso: " << data.country_iso
            << ", indx_source: " << data.indx_source
            << ", seasonality_transformation: " <<
            data.seasonality_transformation
            << " }";
    return os;
}

std::ostream& operator<<(std::ostream& os, const MacroHeadLineEvent& event) {
    os << "MacroHeadLineEvent { event_type: " << eventTypeToString(event.event_type)
       << ", event_subtype: " << eventSubTypeToString(event.event_subtype)
       << ", event_id: " << event.event_id
       << ", observation_period: " << event.observation_period
       << ", release_start_dt: { microseconds: " << event.release_start_dt.microseconds
       << ", offset: " << event.release_start_dt.offset << " }"
       << ", release_end_dt: { microseconds: " << event.release_end_dt.microseconds
       << ", offset: " << event.release_end_dt.offset << " }"
       << ", prior_event_id: " << event.prior_event_id
       << ", prior_observation_period: " << event.prior_observation_period
       << ", prior_economic_release_start_dt: { microseconds: " << event.prior_economic_release_start_dt.microseconds
       << ", offset: " << event.prior_economic_release_start_dt.offset << " }"
       << ", prior_economic_release_end_dt: { microseconds: " << event.prior_economic_release_end_dt.microseconds
       << ", offset: " << event.prior_economic_release_end_dt.offset << " }"
       << ", value: " << event.value
       << " }";
    return os;
}

std::ostream& operator<<(std::ostream& os, const MacroCalendarEvent& event) {
    os << "MacroCalendarEvent { id_bb_global: " << event.id_bb_global
       << ", parsekyable_des: " << event.parsekyable_des
       << ", event_type: " << eventTypeToString(event.event_type)
       << ", event_subtype: " << eventSubTypeToString(event.event_subtype)
       << ", description: " << event.description
       << ", event_id: " << event.event_id
       << ", observation_period: " << event.observation_period
       << ", release_start_dt: { microseconds: " << event.release_start_dt.microseconds
       << ", offset: " << event.release_start_dt.offset << " }"
       << ", release_end_dt: { microseconds: " << event.release_end_dt.microseconds
       << ", offset: " << event.release_end_dt.offset << " }"
       << ", release_status: " << releaseStatusToString(event.release_status)
       << ", relevance_value: " << event.relevance_value
       << " }";
    return os;
}

} // namespace BlpConn
