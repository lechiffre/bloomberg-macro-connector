/**
 * This file contains functions to serialize classes
 * defined in economic_event.h to/from FlatBuffers objects in the
 * namespace FB.
 */

#include <ctime>
#include "blpconn_serialize.h"

static const blpapi::Name SINGLE("SINGLE");
static const blpapi::Name NUMBER("NUMBER");
static const blpapi::Name DISTRIBUTION("DISTRIBUTION");
static const blpapi::Name AVERAGE("AVERAGE");
static const blpapi::Name LOW("LOW");
static const blpapi::Name HIGH("HIGH");
static const blpapi::Name MEDIAN("MEDIAN");
static const blpapi::Name STANDARD_DEVIATION("STANDARD_DEVIATION");
namespace BlpConn {

flatbuffers::Offset<FB::DateTime> serializeDateTime(
    flatbuffers::FlatBufferBuilder& builder, const DateTimeType& dt) {
    return FB::CreateDateTime(builder, dt.microseconds, dt.offset);
}

flatbuffers::Offset<FB::DateTime> extractDateTime(
        flatbuffers::FlatBufferBuilder& builder,
        const blpapi::Datetime& blpDatetime)
{
    std::tm timeStruct = {};
    timeStruct.tm_year = blpDatetime.year() - 1900;
    timeStruct.tm_mon = blpDatetime.month() - 1;   // tm_mon is 0-based
    timeStruct.tm_mday = blpDatetime.day();
    timeStruct.tm_hour = blpDatetime.hours();
    timeStruct.tm_min = blpDatetime.minutes();
    timeStruct.tm_sec = blpDatetime.seconds();

    std::time_t timeSinceEpoch = std::mktime(&timeStruct);
    uint64_t microseconds = static_cast<uint64_t>(timeSinceEpoch) * 1000000 +
        blpDatetime.microseconds();
    return FB::CreateDateTime(builder, microseconds,
            static_cast<int16_t>(blpDatetime.offset()));
}

flatbuffers::Offset<FB::Value> serializeValue(
    flatbuffers::FlatBufferBuilder& builder, const ValueType& value) {
    return FB::CreateValue(builder, value.number, value.value, value.low,
        value.high, value.median, value.average, value.standard_deviation);
}

static double getFloatFromElement(const blpapi::Element& elem) {
    std::string valueStr = elem.getValueAsString();
    try {
        return std::stod(valueStr);
    } catch (const std::invalid_argument&) {
        return std::nan("");
    } catch (const std::out_of_range&) {
        return std::nan("");
    }
}

static int getIntFromElement(const blpapi::Element& elem) {
    std::string valueStr = elem.getValueAsString();
    return std::stoi(valueStr);
}

flatbuffers::Offset<FB::Value> extractValue(
        flatbuffers::FlatBufferBuilder& builder, const blpapi::Element& elem, 
        const blpapi::Name name)
{
    if (!elem.hasElement(name)) {
        return FB::CreateValue(builder, 0, std::nan(""), std::nan(""),
                std::nan(""), std::nan(""), std::nan(""), std::nan(""));
    }

    const blpapi::Element subElem = elem.getElement(name);
    const blpapi::Element choice = subElem.getChoice();

    if (choice.name() == SINGLE) {
        double value = getFloatFromElement(choice);
        return FB::CreateValue(builder, 1, value, value, value, value, value,
                0.0);
    } else if (choice.name() == DISTRIBUTION) {
        int number = choice.hasElement(NUMBER) 
            ? getIntFromElement(choice.getElement(NUMBER)) 
            : 0;
        double average = choice.hasElement(AVERAGE)
            ? getFloatFromElement(choice.getElement(AVERAGE))
            : std::nan("");
        double low = choice.hasElement(LOW) 
            ? getFloatFromElement(choice.getElement(LOW)) 
            : std::nan("");
        double high = choice.hasElement(HIGH) 
            ? getFloatFromElement(choice.getElement(HIGH)) 
            : std::nan("");
        double median = choice.hasElement(MEDIAN) 
            ? getFloatFromElement(choice.getElement(MEDIAN)) 
            : std::nan("");
        double stdDev = choice.hasElement(STANDARD_DEVIATION) 
            ? getFloatFromElement(choice.getElement(STANDARD_DEVIATION)) 
            : std::nan("");
        return FB::CreateValue(builder, number, std::nan(""), low, high, median,
                average, stdDev);
    }

    throw std::runtime_error("Unexpected choice type in extractValue.");
}

flatbuffers::Offset<FB::HeadlineEconomicEvent> serializeHeadlineEconomicEvent(
    flatbuffers::FlatBufferBuilder& builder, const HeadlineEconomicEvent& event) {
    PROFILE_FUNCTION()
    auto id_bb_global = builder.CreateString(event.id_bb_global);
    auto parsekyable_des = builder.CreateString(event.parsekyable_des);
    auto description = builder.CreateString(event.description);
    auto observation_period = builder.CreateString(event.observation_period);
    auto release_start_dt = serializeDateTime(builder, event.release_start_dt);
    auto release_end_dt = serializeDateTime(builder, event.release_end_dt);
    auto prior_observation_period = builder.CreateString(event.prior_observation_period);
    auto value = serializeValue(builder, event.value);
    auto prior_value = serializeValue(builder, event.prior_value);
    auto prior_release_start_dt = serializeDateTime(builder, event.prior_economic_release_start_dt);
    auto prior_release_end_dt = serializeDateTime(builder, event.prior_economic_release_end_dt);
    END_PROFILE_FUNCTION()
    return FB::CreateHeadlineEconomicEvent(
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
        value,
        prior_value,
        event.prior_event_id,
        prior_observation_period,
        prior_release_start_dt,
        prior_release_end_dt
    );
}

flatbuffers::Offset<FB::MacroReferenceData> serializeMacroReferenceData(
        flatbuffers::FlatBufferBuilder& builder,
        const MacroReferenceData& data) {
    PROFILE_FUNCTION()
    auto id_bb_global = builder.CreateString(data.id_bb_global);
    auto parsekyable_des = builder.CreateString(data.parsekyable_des);
    auto description = builder.CreateString(data.description);
    auto indx_freq = builder.CreateString(data.indx_freq);
    auto indx_units = builder.CreateString(data.indx_units);
    auto country_iso = builder.CreateString(data.country_iso);
    auto indx_source = builder.CreateString(data.indx_source);
    auto seasonality_transformation = builder.CreateString(
            data.seasonality_transformation);
    END_PROFILE_FUNCTION()
    return FB::CreateMacroReferenceData(builder,
            data.corr_id,
            id_bb_global,
            parsekyable_des,
            description,
            indx_freq,
            indx_units,
            country_iso,
            indx_source,
            seasonality_transformation);
}

flatbuffers::Offset<FB::MacroHeadlineEvent> serializeMacroHeadlineEvent(
        flatbuffers::FlatBufferBuilder& builder,
        const MacroHeadlineEvent& event) {
    PROFILE_FUNCTION()
    auto observation_period = builder.CreateString(event.observation_period);
    auto release_start_dt = serializeDateTime(builder, event.release_start_dt);
    auto release_end_dt = serializeDateTime(builder, event.release_end_dt);
    auto prior_observation_period = builder.CreateString(
            event.prior_observation_period);
    auto prior_release_start_dt = serializeDateTime(
            builder, event.prior_economic_release_start_dt);
    auto prior_release_end_dt = serializeDateTime(
            builder, event.prior_economic_release_end_dt);
    auto value = serializeValue(builder, event.value);
    END_PROFILE_FUNCTION()
    return FB::CreateMacroHeadlineEvent(
            builder,
            event.corr_id,
            static_cast<FB::EventType>(event.event_type),
            static_cast<FB::EventSubType>(event.event_subtype),
            event.event_id,
            observation_period,
            release_start_dt,
            release_end_dt,
            event.prior_event_id,
            prior_observation_period,
            prior_release_start_dt,
            prior_release_end_dt,
            value);
}

flatbuffers::Offset<FB::MacroCalendarEvent> serializeMacroCalendarEvent(
        flatbuffers::FlatBufferBuilder& builder,
        const MacroCalendarEvent& event) {
    PROFILE_FUNCTION()
    auto id_bb_global = builder.CreateString(event.id_bb_global);
    auto parsekyable_des = builder.CreateString(event.parsekyable_des);
    auto description = builder.CreateString(event.description);
    auto observation_period = builder.CreateString(event.observation_period);
    auto release_start_dt = serializeDateTime(builder, event.release_start_dt);
    auto release_end_dt = serializeDateTime(builder, event.release_end_dt);
    END_PROFILE_FUNCTION()
    return FB::CreateMacroCalendarEvent(
        builder,
        event.corr_id,
        id_bb_global,
        parsekyable_des,
        static_cast<FB::EventType>(event.event_type),
        static_cast<FB::EventSubType>(event.event_subtype),
        description,
        event.event_id,
        observation_period,
        release_start_dt,
        release_end_dt,
        static_cast<FB::ReleaseStatus>(event.release_status),
        event.relevance_value);
}

flatbuffers::Offset<FB::HeadlineCalendarEvent> serializeHeadlineCalendarEvent(
    flatbuffers::FlatBufferBuilder& builder, const HeadlineCalendarEvent& event) {
    PROFILE_FUNCTION()
    auto id_bb_global = builder.CreateString(event.id_bb_global);
    auto parsekyable_des = builder.CreateString(event.parsekyable_des);
    auto description = builder.CreateString(event.description);
    auto observation_period = builder.CreateString(event.observation_period);

    auto release_start_dt = serializeDateTime(builder, event.release_start_dt);
    auto release_end_dt = serializeDateTime(builder, event.release_end_dt);
    END_PROFILE_FUNCTION()
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
    PROFILE_FUNCTION()
    auto message = builder.CreateString(log_message.message);
    auto log_dt = serializeDateTime(builder, log_message.log_dt);
    END_PROFILE_FUNCTION()
    return FB::CreateLogMessage(
            builder,
            log_dt,
            log_message.module, // Fix: Serialize the module field
            log_message.status,
            log_message.correlation_id,
            message); // Fix: Serialize the correlation_id field
}

flatbuffers::FlatBufferBuilder buildBufferMacroReferenceData(
        MacroReferenceData& data) {
    PROFILE_FUNCTION()
    flatbuffers::FlatBufferBuilder builder;
    auto fb_macro_data = serializeMacroReferenceData(builder, data).Union();
    auto fb_main = FB::CreateMain(builder,
            FB::Message::Message_MacroReferenceData, fb_macro_data);
    builder.Finish(fb_main);
    END_PROFILE_FUNCTION()
    return builder;
}

flatbuffers::FlatBufferBuilder buildBufferMacroHeadlineEvent(
        MacroHeadlineEvent& event) {
    PROFILE_FUNCTION()
    flatbuffers::FlatBufferBuilder builder;
    auto fb_macro_headline = serializeMacroHeadlineEvent(builder, event).Union();
    auto fb_main = FB::CreateMain(builder,
            FB::Message::Message_MacroHeadlineEvent, fb_macro_headline);
    builder.Finish(fb_main);
    END_PROFILE_FUNCTION()
    return builder;
}

flatbuffers::FlatBufferBuilder buildBufferMacroCalendarEvent(
        MacroCalendarEvent& event) {
    PROFILE_FUNCTION()
    flatbuffers::FlatBufferBuilder builder;
    auto fb_macro_calendar = serializeMacroCalendarEvent(builder, event).Union();
    auto fb_main = FB::CreateMain(builder,
        FB::Message::Message_MacroCalendarEvent, fb_macro_calendar);
    builder.Finish(fb_main);
    END_PROFILE_FUNCTION()
    return builder;
}

flatbuffers::FlatBufferBuilder buildBufferEconomicEvent(HeadlineEconomicEvent& event) {
    PROFILE_FUNCTION()
    flatbuffers::FlatBufferBuilder builder;
    auto fb_economic_event = serializeHeadlineEconomicEvent(builder, event).Union();
    auto fb_main = FB::CreateMain(builder, 
        FB::Message::Message_HeadlineEconomicEvent, fb_economic_event);
    builder.Finish(fb_main);
    END_PROFILE_FUNCTION()
    return builder;
}

flatbuffers::FlatBufferBuilder buildBufferCalendarEvent(HeadlineCalendarEvent& event) {
    PROFILE_FUNCTION()
    flatbuffers::FlatBufferBuilder builder;
    auto fb_calendar_event = serializeHeadlineCalendarEvent(builder, event).Union();
    auto fb_main = FB::CreateMain(builder,
        FB::Message::Message_HeadlineCalendarEvent, fb_calendar_event);
    builder.Finish(fb_main);
    END_PROFILE_FUNCTION()
    return builder;
}

flatbuffers::FlatBufferBuilder buildBufferLogMessage(LogMessage& log_message) {
    PROFILE_FUNCTION()
    flatbuffers::FlatBufferBuilder builder;
    auto fb_log_message = serializeLogMessage(builder, log_message).Union();
    auto fb_main = FB::CreateMain(builder, FB::Message::Message_LogMessage, fb_log_message);
    builder.Finish(fb_main);
    END_PROFILE_FUNCTION()
    return builder;
}

} // namespace BlpConn
