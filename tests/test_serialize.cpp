#include <vector>
#include <gtest/gtest.h>
#include <flatbuffers/flatbuffers.h>
#include <blpconn_message.h>
#include <blpconn_serialize.h>
#include <blpconn_fb_generated.h>

// Helper function to create test data for HeadlineEconomicEvent
std::vector<BlpConn::HeadlineEconomicEvent> createEconomicEventTestData() {
    std::vector<BlpConn::HeadlineEconomicEvent> events;

    // Valid case
    BlpConn::HeadlineEconomicEvent valid_event;
    valid_event.id_bb_global = "BBG002SBJ900";
    valid_event.parsekyable_des = "TEST_INDEX_0";
    valid_event.description = "Valid Description";
    valid_event.event_type = BlpConn::EventType::Actual;
    valid_event.event_subtype = BlpConn::EventSubType::New;
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
    BlpConn::HeadlineEconomicEvent missing_id_event = valid_event;
    missing_id_event.id_bb_global = "";
    events.push_back(missing_id_event);

    // Failure case: Invalid event_type
    BlpConn::HeadlineEconomicEvent invalid_type_event = valid_event;
    invalid_type_event.event_type = static_cast<BlpConn::EventType>(999); // Invalid enum value
    events.push_back(invalid_type_event);

    return events;
}

// Helper function to create test data for HeadlineCalendarEvent
std::vector<BlpConn::HeadlineCalendarEvent> createCalendarEventTestData() {
    std::vector<BlpConn::HeadlineCalendarEvent> events;

    // Valid case
    BlpConn::HeadlineCalendarEvent valid_event;
    valid_event.id_bb_global = "BBG002SBQ800";
    valid_event.parsekyable_des = "CALENDAR_INDEX_0";
    valid_event.description = "Valid Calendar Description";
    valid_event.event_type = BlpConn::EventType::Calendar;
    valid_event.event_subtype = BlpConn::EventSubType::Update;
    valid_event.event_id = 2000;
    valid_event.observation_period = "Calendar Period";
    valid_event.release_start_dt = {1773941400000000, 0};
    valid_event.release_end_dt = {1773941400000000, 0};
    valid_event.release_status = BlpConn::ReleaseStatus::Scheduled;
    events.push_back(valid_event);

    // Failure case: Missing parsekyable_des
    BlpConn::HeadlineCalendarEvent missing_parsekyable_event = valid_event;
    missing_parsekyable_event.parsekyable_des = "";
    events.push_back(missing_parsekyable_event);

    // Failure case: Invalid release_status
    BlpConn::HeadlineCalendarEvent invalid_status_event = valid_event;
    invalid_status_event.release_status = static_cast<BlpConn::ReleaseStatus>(999); // Invalid enum value
    events.push_back(invalid_status_event);

    return events;
}

// Parameterized test for HeadlineEconomicEvent
class EconomicEventTest : public ::testing::TestWithParam<BlpConn::HeadlineEconomicEvent> {};

TEST_P(EconomicEventTest, SerializeAndDeserialize) {
    const auto& original_event = GetParam();

    // Serialize the object
    flatbuffers::FlatBufferBuilder builder;
    auto serialized_event = serializeHeadlineEconomicEvent(builder, original_event);
    builder.Finish(serialized_event);

    // Deserialize the object
    auto fb_event = flatbuffers::GetRoot<BlpConn::FB::HeadlineEconomicEvent>(builder.GetBufferPointer());
    BlpConn::HeadlineEconomicEvent deserialized_event = BlpConn::toHeadlineEconomicEvent(fb_event);

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
class CalendarEventTest : public ::testing::TestWithParam<BlpConn::HeadlineCalendarEvent> {};

TEST_P(CalendarEventTest, SerializeAndDeserialize) {
    const auto& original_event = GetParam();

    // Serialize the object
    flatbuffers::FlatBufferBuilder builder;
    auto serialized_event = serializeHeadlineCalendarEvent(builder, original_event);
    builder.Finish(serialized_event);

    // Deserialize the object
    auto fb_event = flatbuffers::GetRoot<BlpConn::FB::HeadlineCalendarEvent>(builder.GetBufferPointer());
    BlpConn::HeadlineCalendarEvent deserialized_event = BlpConn::toHeadlineCalendarEvent(fb_event);

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

// Instantiate tests with test data
INSTANTIATE_TEST_SUITE_P(EconomicEventTests, EconomicEventTest, ::testing::ValuesIn(createEconomicEventTestData()));
INSTANTIATE_TEST_SUITE_P(CalendarEventTests, CalendarEventTest, ::testing::ValuesIn(createCalendarEventTestData()));

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
