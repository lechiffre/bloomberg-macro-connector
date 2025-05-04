/**
 * This file contains functions to serialize and deserialize classes
 * defined in economic_event.h to/from FlatBuffers objects in the
 * namespace BlpConn::FB.
 */

#include <blpconn_message.h>

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

flatbuffers::Offset<BlpConn::FB::LogMessage> serializeLogMessage(
    flatbuffers::FlatBufferBuilder& builder, const BlpConn::LogMessage& log_message) {
    auto message = builder.CreateString(log_message.message);
    auto module_name = builder.CreateString(log_message.module_name);
    auto log_dt = serializeDateTime(builder, log_message.log_dt);
    return BlpConn::FB::CreateLogMessage(builder, log_dt, module_name, message);
}

flatbuffers::FlatBufferBuilder buildBufferEconomicEvent(HeadlineEconomicEvent& event) {
    flatbuffers::FlatBufferBuilder builder;
    auto fb_economic_event = serializeHeadlineEconomicEvent(builder, event).Union();
    auto fb_main = BlpConn::FB::CreateMain(builder, BlpConn::FB::Message::Message_HeadlineEconomicEvent, fb_economic_event);
    builder.Finish(fb_main);
    return builder;
}


} // namespace BlpConn
