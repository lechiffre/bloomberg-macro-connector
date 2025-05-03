/**
 * This file contains functions to serialize and deserialize classes
 * defined in economic_event.h to/from FlatBuffers objects in the
 * namespace BlpConn::FB.
 */

#include <economic_event.h>

namespace BlpConn {

flatbuffers::Offset<BlpConn::FB::DateTime> serializeDateTime(
    flatbuffers::FlatBufferBuilder& builder, const BlpConn::DateTimeType& dt) {
    return BlpConn::FB::CreateDateTime(builder, dt.microseconds, dt.offset);
}

flatbuffers::Offset<BlpConn::FB::Value> serializeValue(
    flatbuffers::FlatBufferBuilder& builder, const BlpConn::ValueType& value) {
    return BlpConn::FB::CreateValue(builder, value.number, value.value, value.low,
                                    value.high, value.median, value.average, value.standard_deviation);
}

flatbuffers::Offset<BlpConn::FB::HeadlineEconomicEvent> serializeHeadlineEconomicEvent(
    flatbuffers::FlatBufferBuilder& builder, const BlpConn::HeadlineEconomicEvent& event) {
    auto id_bb_global = builder.CreateString(event.id_bb_global);
    auto parsekyable_des = builder.CreateString(event.parsekyable_des);
    auto description = builder.CreateString(event.description);
    auto observation_period = builder.CreateString(event.observation_period);
    auto prior_observation_period = builder.CreateString(event.prior_observation_period);

    auto release_start_dt = serializeDateTime(builder, event.release_start_dt);
    auto release_end_dt = serializeDateTime(builder, event.release_end_dt);
    auto value = serializeValue(builder, event.value);
    auto prior_value = serializeValue(builder, event.prior_value);
    auto prior_release_start_dt = serializeDateTime(builder, event.prior_economic_release_start_dt);
    auto prior_release_end_dt = serializeDateTime(builder, event.prior_economic_release_end_dt);

    return BlpConn::FB::CreateHeadlineEconomicEvent(
        builder, id_bb_global, parsekyable_des, description, static_cast<BlpConn::FB::EventType>(event.event_type),
        static_cast<BlpConn::FB::EventSubType>(event.event_subtype), event.event_id, observation_period,
        release_start_dt, release_end_dt, value, prior_value, event.prior_event_id, prior_observation_period,
        prior_release_start_dt, prior_release_end_dt);
}

flatbuffers::Offset<BlpConn::FB::HeadlineCalendarEvent> serializeHeadlineCalendarEvent(
    flatbuffers::FlatBufferBuilder& builder, const BlpConn::HeadlineCalendarEvent& event) {
    auto id_bb_global = builder.CreateString(event.id_bb_global);
    auto parsekyable_des = builder.CreateString(event.parsekyable_des);
    auto description = builder.CreateString(event.description);
    auto observation_period = builder.CreateString(event.observation_period);

    auto release_start_dt = serializeDateTime(builder, event.release_start_dt);
    auto release_end_dt = serializeDateTime(builder, event.release_end_dt);

    return BlpConn::FB::CreateHeadlineCalendarEvent(
        builder, id_bb_global, parsekyable_des, description, static_cast<BlpConn::FB::EventType>(event.event_type),
        static_cast<BlpConn::FB::EventSubType>(event.event_subtype), event.event_id, observation_period,
        release_start_dt, release_end_dt, static_cast<BlpConn::FB::ReleaseStatus>(event.release_status));
}

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

} // namespace BlpConn