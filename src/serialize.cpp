/**
 * This file contains functions to serialize and deserialize classes
 * defined in economic_event.h to/from FlatBuffers objects in the
 * namespace FB.
 */

#include <blpconn_message.h>
#include <blpconn_fb_generated.h>

namespace BlpConn {

flatbuffers::Offset<FB::DateTime> serializeDateTime(
    flatbuffers::FlatBufferBuilder& builder, const DateTimeType& dt) {
    return FB::CreateDateTime(builder, dt.microseconds, dt.offset);
}

flatbuffers::Offset<FB::Value> serializeValue(
    flatbuffers::FlatBufferBuilder& builder, const ValueType& value) {
    return FB::CreateValue(builder, value.number, value.value, value.low,
                                    value.high, value.median, value.average, value.standard_deviation);
}

flatbuffers::Offset<FB::HeadlineEconomicEvent> serializeHeadlineEconomicEvent(
    flatbuffers::FlatBufferBuilder& builder, const HeadlineEconomicEvent& event) {
    auto id_bb_global = builder.CreateString(event.id_bb_global);
    auto parsekyable_des = builder.CreateString(event.parsekyable_des);
    auto description = builder.CreateString(event.description);
    auto observation_period = builder.CreateString(event.observation_period);
    auto release_start_dt = serializeDateTime(builder, event.release_start_dt);
    auto release_end_dt = serializeDateTime(builder, event.release_end_dt);
    auto prior_observation_period = builder.CreateString(event.prior_observation_period);

    auto value = serializeValue(builder, event.value);
    auto prior_value = serializeValue(builder, event.prior_value);
    auto prior_release_start_dt = serializeDateTime(
        builder, event.prior_economic_release_start_dt);
    auto prior_release_end_dt = serializeDateTime(
        builder, event.prior_economic_release_end_dt);

    return FB::CreateHeadlineEconomicEvent(
        builder,
        id_bb_global,
        parsekyable_des,
        description,
        static_cast<FB::EventType>(event.event_type),
        static_cast<FB::EventSubType>(event.event_subtype),
        event.event_id, observation_period,
        release_start_dt,
        release_end_dt,
        value,
        prior_value,
        event.prior_event_id,
        prior_observation_period,
        prior_release_start_dt,
        prior_release_end_dt);
}

flatbuffers::Offset<FB::HeadlineCalendarEvent> serializeHeadlineCalendarEvent(
    flatbuffers::FlatBufferBuilder& builder, const HeadlineCalendarEvent& event) {
    auto id_bb_global = builder.CreateString(event.id_bb_global);
    auto parsekyable_des = builder.CreateString(event.parsekyable_des);
    auto description = builder.CreateString(event.description);
    auto observation_period = builder.CreateString(event.observation_period);

    auto release_start_dt = serializeDateTime(builder, event.release_start_dt);
    auto release_end_dt = serializeDateTime(builder, event.release_end_dt);
    return FB::CreateHeadlineCalendarEvent(
        builder,
        id_bb_global,
        parsekyable_des,
        description,
        static_cast<FB::EventType>(event.event_type),
        static_cast<FB::EventSubType>(event.event_subtype),
        event.event_id,
        observation_period,
        release_start_dt,
        release_end_dt,
        static_cast<FB::ReleaseStatus>(event.release_status));
}

flatbuffers::Offset<FB::LogMessage> serializeLogMessage(
    flatbuffers::FlatBufferBuilder& builder, const LogMessage& log_message) {
    auto message = builder.CreateString(log_message.message);
    auto module_name = builder.CreateString(log_message.module_name);
    auto log_dt = serializeDateTime(builder, log_message.log_dt);
    return FB::CreateLogMessage(builder, log_dt, module_name, message);
}

flatbuffers::FlatBufferBuilder buildBufferEconomicEvent(HeadlineEconomicEvent& event) {
    flatbuffers::FlatBufferBuilder builder;
    auto fb_economic_event = serializeHeadlineEconomicEvent(builder, event).Union();
    auto fb_main = FB::CreateMain(builder, 
        FB::Message::Message_HeadlineEconomicEvent, fb_economic_event);
    builder.Finish(fb_main);
    return builder;
}

flatbuffers::FlatBufferBuilder buildBufferCalendarEvent(HeadlineCalendarEvent& event) {
    flatbuffers::FlatBufferBuilder builder;
    auto fb_calendar_event = serializeHeadlineCalendarEvent(builder, event).Union();
    auto fb_main = FB::CreateMain(builder,
        FB::Message::Message_HeadlineCalendarEvent, fb_calendar_event);
    builder.Finish(fb_main);
    return builder;
}

flatbuffers::FlatBufferBuilder buildBufferLogMessage(LogMessage& log_message) {
    flatbuffers::FlatBufferBuilder builder;
    auto fb_log_message = serializeLogMessage(builder, log_message).Union();
    auto fb_main = FB::CreateMain(builder, FB::Message::Message_LogMessage, fb_log_message);
    builder.Finish(fb_main);
    return builder;
}

} // namespace BlpConn
