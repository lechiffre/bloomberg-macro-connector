#include "blpconn_observer.h"
#include "blpconn_fb_generated.h"
#include "blpconn_deserialize.h"

namespace BlpConn {

BlpConn::DateTimeType deserializeDateTime(const BlpConn::FB::DateTime* dt) {
    BlpConn::DateTimeType result;
    result.microseconds = dt->micros();
    result.offset = dt->offset();
    return result;
}

BlpConn::ValueType deserializeValue(const BlpConn::FB::Value* value) {
    BlpConn::ValueType result;
    result.number = value->number();
    result.value = value->value();
    result.low = value->low();
    result.high = value->high();
    result.median = value->median();
    result.average = value->average();
    result.standard_deviation = value->standard_deviation();
    return result;
}

BlpConn::HeadlineEconomicEvent toHeadlineEconomicEvent(const BlpConn::FB::HeadlineEconomicEvent* fb_event) {
    BlpConn::HeadlineEconomicEvent event;
    event.id_bb_global = fb_event->id_bb_global()->str();
    event.parsekyable_des = fb_event->parsekyable_des()->str();
    event.description = fb_event->description()->str();
    event.event_type = static_cast<BlpConn::EventType>(fb_event->event_type());
    event.event_subtype = static_cast<BlpConn::EventSubType>(fb_event->event_subtype());
    event.event_id = fb_event->event_id();
    event.observation_period = fb_event->observation_period()->str();
    event.release_start_dt = deserializeDateTime(fb_event->release_start_dt());
    event.release_end_dt = deserializeDateTime(fb_event->release_end_dt());
    event.value = deserializeValue(fb_event->value());
    event.prior_value = deserializeValue(fb_event->prior_value());
    event.prior_event_id = fb_event->prior_event_id();
    event.prior_observation_period = fb_event->prior_observation_period()->str();
    event.prior_economic_release_start_dt = deserializeDateTime(fb_event->prior_economic_release_start_dt());
    event.prior_economic_release_end_dt = deserializeDateTime(fb_event->prior_economic_release_end_dt());
    return event;
}

BlpConn::HeadlineCalendarEvent toHeadlineCalendarEvent(const BlpConn::FB::HeadlineCalendarEvent* fb_event) {
    BlpConn::HeadlineCalendarEvent event;
    event.id_bb_global = fb_event->id_bb_global()->str();
    event.parsekyable_des = fb_event->parsekyable_des()->str();
    event.description = fb_event->description()->str();
    event.event_type = static_cast<BlpConn::EventType>(fb_event->event_type());
    event.event_subtype = static_cast<BlpConn::EventSubType>(fb_event->event_subtype());
    event.event_id = fb_event->event_id();
    event.observation_period = fb_event->observation_period()->str();
    event.release_start_dt = deserializeDateTime(fb_event->release_start_dt());
    event.release_end_dt = deserializeDateTime(fb_event->release_end_dt());
    event.release_status = static_cast<BlpConn::ReleaseStatus>(fb_event->release_status());
    return event;
}

LogMessage toLogMessage(const BlpConn::FB::LogMessage* fb_log_message) {
    BlpConn::LogMessage log_message;
    log_message.log_dt = deserializeDateTime(fb_log_message->log_dt());
    log_message.module = fb_log_message->module_(); // Fix: Use correct field name 'module_'
    log_message.status = fb_log_message->status();
    log_message.correlation_id = fb_log_message->correlation_id();
    log_message.message = fb_log_message->message()->str();
    return log_message;
}

void defaultObserver(const uint8_t *buffer, size_t size) {
    flatbuffers::Verifier verifier(buffer, size);
    if (!BlpConn::FB::VerifyMessageVector(verifier, nullptr, nullptr)) {
        std::cout << "Invalid message" << std::endl;
        return;
    }
    auto main = flatbuffers::GetRoot<BlpConn::FB::Main>(buffer);
    if (main->message_type() == BlpConn::FB::Message_HeadlineEconomicEvent) {
        auto fb_event = main->message_as_HeadlineEconomicEvent();
        auto event = toHeadlineEconomicEvent(fb_event);
        std::cout << event << std::endl;
    } else if (main->message_type() == FB::Message_HeadlineCalendarEvent) {
        auto fb_event = main->message_as_HeadlineCalendarEvent();
        auto event = toHeadlineCalendarEvent(fb_event);
        std::cout << event << std::endl;
    } else if (main->message_type() == BlpConn::FB::Message_LogMessage) {
        auto fb_log_message = main->message_as_LogMessage();
        auto log_message = toLogMessage(fb_log_message);
        std::cout << log_message << std::endl;
    }
}



} // namespace BlpConn
