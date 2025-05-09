#include <vector>
#include <gtest/gtest.h>
#include <flatbuffers/flatbuffers.h>
#include <blpconn_serialize.h>
#include <blpconn_deserialize.h>

using namespace BlpConn;

std::vector<HeadlineEconomicEvent> createEconomicEventTestData() {
    std::vector<HeadlineEconomicEvent> events;

    // Valid case
    HeadlineEconomicEvent valid_event;
    valid_event.id_bb_global = "BBG002SBJ900";
    valid_event.parsekyable_des = "TEST_INDEX_0";
    valid_event.description = "Valid Description";
    valid_event.event_type = EventType::Actual;
    valid_event.event_subtype = EventSubType::New;
    valid_event.event_id = 1000;
    valid_event.observation_period = "Valid Period";
    valid_event.release_start_dt = {1743701400000000, 0};
    valid_event.release_end_dt = {1743701400000000, 0};
    valid_event.value = {1, -1.52, NAN, NAN, NAN, NAN, NAN};
    valid_event.prior_value = {NAN, NAN, NAN, NAN, NAN, NAN, NAN};
    valid_event.prior_event_id = 999;
    valid_event.prior_observation_period = "Prior Period";
    valid_event.prior_economic_release_start_dt = {0, 0};
    valid_event.prior_economic_release_end_dt = {0, 0};
    events.push_back(valid_event);

    // Failure case: Missing id_bb_global
    HeadlineEconomicEvent missing_id_event = valid_event;
    missing_id_event.id_bb_global = "";
    events.push_back(missing_id_event);

    // Failure case: Invalid event_type
    HeadlineEconomicEvent invalid_type_event = valid_event;
    invalid_type_event.event_type = static_cast<EventType>(999); // Invalid enum value
    events.push_back(invalid_type_event);

    return events;
}

std::vector<HeadlineCalendarEvent> createCalendarEventTestData() {
    std::vector<HeadlineCalendarEvent> events;

    // Valid case
    HeadlineCalendarEvent valid_event;
    valid_event.id_bb_global = "BBG002SBQ800";
    valid_event.parsekyable_des = "CALENDAR_INDEX_0";
    valid_event.description = "Valid Calendar Description";
    valid_event.event_type = EventType::Calendar;
    valid_event.event_subtype = EventSubType::Update;
    valid_event.event_id = 2000;
    valid_event.observation_period = "Calendar Period";
    valid_event.release_start_dt = {1773941400000000, 0};
    valid_event.release_end_dt = {1773941400000000, 0};
    valid_event.release_status = ReleaseStatus::Scheduled;
    events.push_back(valid_event);

    // Failure case: Missing parsekyable_des
    HeadlineCalendarEvent missing_parsekyable_event = valid_event;
    missing_parsekyable_event.parsekyable_des = "";
    events.push_back(missing_parsekyable_event);

    // Failure case: Invalid release_status
    HeadlineCalendarEvent invalid_status_event = valid_event;
    invalid_status_event.release_status = static_cast<ReleaseStatus>(999); // Invalid enum value
    events.push_back(invalid_status_event);

    return events;
}

std::vector<LogMessage> createLogMessageTestData() {
    std::vector<LogMessage> messages;

    // Valid case
    LogMessage valid_message;
    valid_message.log_dt = {1743701400000000, 0};
    valid_message.module = 1;
    valid_message.status = 0;
    valid_message.correlation_id = 12345;
    valid_message.message = "Valid log message";
    messages.push_back(valid_message);

    // Failure case: Empty message
    LogMessage empty_message = valid_message;
    empty_message.message = "";
    messages.push_back(empty_message);

    // Failure case: Invalid module
    LogMessage invalid_module_message = valid_message;
    invalid_module_message.module = -1; // Invalid module
    messages.push_back(invalid_module_message);

    return messages;
}

// Parameterized test for HeadlineEconomicEvent
class EconomicEventTest : public ::testing::TestWithParam<HeadlineEconomicEvent> {};

TEST_P(EconomicEventTest, SerializeAndDeserialize) {
    const auto& original_event = GetParam();

    // Serialize the object
    flatbuffers::FlatBufferBuilder builder;
    auto serialized_event = serializeHeadlineEconomicEvent(builder, original_event);
    builder.Finish(serialized_event);

    // Deserialize the object
    auto fb_event = flatbuffers::GetRoot<FB::HeadlineEconomicEvent>(builder.GetBufferPointer());
    HeadlineEconomicEvent deserialized_event = toHeadlineEconomicEvent(fb_event);

    // Validate the deserialized object
    EXPECT_EQ(deserialized_event.id_bb_global, original_event.id_bb_global);
    EXPECT_EQ(deserialized_event.parsekyable_des, original_event.parsekyable_des);
    EXPECT_EQ(deserialized_event.description, original_event.description);
    EXPECT_EQ(deserialized_event.event_type, original_event.event_type);
    EXPECT_EQ(deserialized_event.event_subtype, original_event.event_subtype);
    EXPECT_EQ(deserialized_event.event_id, original_event.event_id);
    EXPECT_EQ(deserialized_event.observation_period, original_event.observation_period);
    EXPECT_EQ(deserialized_event.release_start_dt.microseconds, original_event.release_start_dt.microseconds);
    EXPECT_EQ(deserialized_event.release_start_dt.offset, original_event.release_start_dt.offset);
    EXPECT_EQ(deserialized_event.release_end_dt.microseconds, original_event.release_end_dt.microseconds);
    EXPECT_EQ(deserialized_event.release_end_dt.offset, original_event.release_end_dt.offset);
    EXPECT_EQ(deserialized_event.value.number, original_event.value.number);
    EXPECT_NEAR(deserialized_event.value.value, original_event.value.value, 1e-6);
    EXPECT_TRUE(std::isnan(deserialized_event.value.low));
    EXPECT_TRUE(std::isnan(deserialized_event.value.high));
    EXPECT_TRUE(std::isnan(deserialized_event.prior_value.number));
    EXPECT_TRUE(std::isnan(deserialized_event.prior_value.value));
    EXPECT_EQ(deserialized_event.prior_event_id, original_event.prior_event_id);
    EXPECT_EQ(deserialized_event.prior_observation_period, original_event.prior_observation_period);
}

// Parameterized test for HeadlineCalendarEvent
class CalendarEventTest : public ::testing::TestWithParam<HeadlineCalendarEvent> {};

TEST_P(CalendarEventTest, SerializeAndDeserialize) {
    const auto& original_event = GetParam();

    // Serialize the object
    flatbuffers::FlatBufferBuilder builder;
    auto serialized_event = serializeHeadlineCalendarEvent(builder, original_event);
    builder.Finish(serialized_event);

    // Deserialize the object
    auto fb_event = flatbuffers::GetRoot<FB::HeadlineCalendarEvent>(builder.GetBufferPointer());
    HeadlineCalendarEvent deserialized_event = toHeadlineCalendarEvent(fb_event);

    // Validate the deserialized object
    EXPECT_EQ(deserialized_event.id_bb_global, original_event.id_bb_global);
    EXPECT_EQ(deserialized_event.parsekyable_des, original_event.parsekyable_des);
    EXPECT_EQ(deserialized_event.description, original_event.description);
    EXPECT_EQ(deserialized_event.event_type, original_event.event_type);
    EXPECT_EQ(deserialized_event.event_subtype, original_event.event_subtype);
    EXPECT_EQ(deserialized_event.event_id, original_event.event_id);
    EXPECT_EQ(deserialized_event.observation_period, original_event.observation_period);
    EXPECT_EQ(deserialized_event.release_start_dt.microseconds, original_event.release_start_dt.microseconds);
    EXPECT_EQ(deserialized_event.release_start_dt.offset, original_event.release_start_dt.offset);
    EXPECT_EQ(deserialized_event.release_end_dt.microseconds, original_event.release_end_dt.microseconds);
    EXPECT_EQ(deserialized_event.release_end_dt.offset, original_event.release_end_dt.offset);
    EXPECT_EQ(deserialized_event.release_status, original_event.release_status);
}

// Parameterized test for LogMessage
class LogMessageTest : public ::testing::TestWithParam<LogMessage> {};

TEST_P(LogMessageTest, SerializeAndDeserialize) {
    const auto& original_message = GetParam();

    // Serialize the object
    flatbuffers::FlatBufferBuilder builder;
    auto serialized_message = serializeLogMessage(builder, original_message);
    builder.Finish(serialized_message);

    // Deserialize the object
    auto fb_message = flatbuffers::GetRoot<FB::LogMessage>(builder.GetBufferPointer());
    LogMessage deserialized_message = toLogMessage(fb_message);

    // Validate the deserialized object
    EXPECT_EQ(deserialized_message.log_dt.microseconds, original_message.log_dt.microseconds);
    EXPECT_EQ(deserialized_message.log_dt.offset, original_message.log_dt.offset);
    EXPECT_EQ(deserialized_message.module, original_message.module); // Fix: Validate module field
    EXPECT_EQ(deserialized_message.status, original_message.status);
    EXPECT_EQ(deserialized_message.correlation_id, original_message.correlation_id); // Fix: Validate correlation_id field
    EXPECT_EQ(deserialized_message.message, original_message.message);
}

// Instantiate tests with test data
INSTANTIATE_TEST_SUITE_P(EconomicEventTests, EconomicEventTest, ::testing::ValuesIn(createEconomicEventTestData()));
INSTANTIATE_TEST_SUITE_P(CalendarEventTests, CalendarEventTest, ::testing::ValuesIn(createCalendarEventTestData()));
INSTANTIATE_TEST_SUITE_P(LogMessageTests, LogMessageTest, ::testing::ValuesIn(createLogMessageTestData()));

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
