#include "blpconn_observer.h"
#include "blpconn_fb_generated.h"
#include "blpconn_deserialize.h"

namespace BlpConn {

DateTimeType deserializeDateTime(const BlpConn::FB::DateTime* dt) {
    BlpConn::DateTimeType result;
    result.microseconds = dt->micros();
    result.offset = dt->offset();
    return result;
}

ValueType deserializeValue(const BlpConn::FB::Value* value) {
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

HeadlineEconomicEvent toHeadlineEconomicEvent(const BlpConn::FB::HeadlineEconomicEvent* fb_event) {
    PROFILE_FUNCTION()
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
    END_PROFILE_FUNCTION()
    return event;
}

HeadlineCalendarEvent toHeadlineCalendarEvent(const BlpConn::FB::HeadlineCalendarEvent* fb_event) {
    PROFILE_FUNCTION()
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
    END_PROFILE_FUNCTION()
    return event;
}

MacroReferenceData toMacroReferenceData(
        const BlpConn::FB::MacroReferenceData* fb_data) {
    PROFILE_FUNCTION()
    BlpConn::MacroReferenceData data;
    data.corr_id = fb_data->corr_id();
    data.id_bb_global = fb_data->id_bb_global()->str();
    data.parsekyable_des = fb_data->parsekyable_des()->str();
    data.description = fb_data->description()->str();
    data.indx_freq = fb_data->indx_freq()->str();
    data.indx_units = fb_data->indx_units()->str();
    data.country_iso = fb_data->country_iso()->str();
    data.indx_source = fb_data->indx_source()->str();
    data.seasonality_transformation = fb_data->seasonality_transformation()->str();
    END_PROFILE_FUNCTION()
    return data;
}

MacroHeadlineEvent toMacroHeadlineEvent(
        const BlpConn::FB::MacroHeadlineEvent* fb_event) {
    PROFILE_FUNCTION()
    BlpConn::MacroHeadlineEvent event;
    event.corr_id = fb_event->corr_id();
    event.event_type = static_cast<BlpConn::EventType>(
            fb_event->event_type());
    event.event_subtype = static_cast<BlpConn::EventSubType>(
            fb_event->event_subtype());
    event.event_id = fb_event->event_id();
    event.observation_period = fb_event->observation_period()->str();
    event.release_start_dt = deserializeDateTime(
            fb_event->release_start_dt());
    event.release_end_dt = deserializeDateTime(
            fb_event->release_end_dt());
    event.prior_event_id = fb_event->prior_event_id();
    event.prior_observation_period = fb_event->prior_observation_period()->str();
    event.prior_economic_release_start_dt = deserializeDateTime(
            fb_event->prior_economic_release_start_dt());
    event.prior_economic_release_end_dt = deserializeDateTime(
            fb_event->prior_economic_release_end_dt());
    event.value = deserializeValue(fb_event->value());
    END_PROFILE_FUNCTION()
    return event;
}

MacroCalendarEvent toMacroCalendarEvent(
        const BlpConn::FB::MacroCalendarEvent* fb_event) {
    PROFILE_FUNCTION()
    BlpConn::MacroCalendarEvent event;
    event.corr_id = fb_event->corr_id();
    event.id_bb_global = fb_event->id_bb_global()->str();
    event.parsekyable_des = fb_event->parsekyable_des()->str();
    event.event_type = static_cast<BlpConn::EventType>(
            fb_event->event_type());
    event.event_subtype = static_cast<BlpConn::EventSubType>(
            fb_event->event_subtype());
    event.description = fb_event->description()->str();
    event.event_id = fb_event->event_id();
    event.observation_period = fb_event->observation_period()->str();
    event.release_start_dt = deserializeDateTime(
            fb_event->release_start_dt());
    event.release_end_dt = deserializeDateTime(
            fb_event->release_end_dt());
    event.release_status = static_cast<BlpConn::ReleaseStatus>(
            fb_event->release_status());
    event.relevance_value = fb_event->relevance_value();
    END_PROFILE_FUNCTION()
    return event;
}

LogMessage toLogMessage(const BlpConn::FB::LogMessage* fb_log_message) {
    PROFILE_FUNCTION()
    BlpConn::LogMessage log_message;
    log_message.log_dt = deserializeDateTime(fb_log_message->log_dt());
    log_message.module = fb_log_message->module_();
    log_message.status = fb_log_message->status();
    log_message.correlation_id = fb_log_message->corr_id();
    log_message.message = fb_log_message->message()->str();
    END_PROFILE_FUNCTION()
    return log_message;
}


} // namespace BlpConn
