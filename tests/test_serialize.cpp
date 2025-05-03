#include <gtest/gtest.h>
#include <flatbuffers/flatbuffers.h>
#include <economic_event.h>
#include <economic_event_generated.h>
#include <vector>

// Helper function to create test data for HeadlineEconomicEvent
std::vector<BlpConn::HeadlineEconomicEvent> createEconomicEventTestData() {
    std::vector<BlpConn::HeadlineEconomicEvent> events;

    for (int i = 0; i < 25; ++i) {
        BlpConn::HeadlineEconomicEvent event;
        event.id_bb_global = "BBG002SBJ" + std::to_string(900 + i);
        event.parsekyable_des = "TEST_INDEX_" + std::to_string(i);
        event.description = "Test Description " + std::to_string(i);
        event.event_type = static_cast<BlpConn::EventType>(i % 5);
        event.event_subtype = static_cast<BlpConn::EventSubType>(i % 5);
        event.event_id = 1000 + i;
        event.observation_period = "Period " + std::to_string(i);
        event.release_start_dt = {static_cast<uint64_t>(1743701400000000 + i * 1000), static_cast<short>(i % 24)};
        event.release_end_dt = {static_cast<uint64_t>(1743701400000000 + i * 2000), static_cast<short>(i % 24)};
        event.value = {static_cast<double>(i), i * 1.1, NAN, NAN, NAN, NAN, NAN};
        event.prior_value = {NAN, NAN, NAN, NAN, NAN, NAN, NAN};
        event.prior_event_id = i - 1;
        event.prior_observation_period = "Prior Period " + std::to_string(i - 1);
        event.prior_economic_release_start_dt = {0, 0};
        event.prior_economic_release_end_dt = {0, 0};
        events.push_back(event);
    }

    return events;
}

// Helper function to create test data for HeadlineCalendarEvent
std::vector<BlpConn::HeadlineCalendarEvent> createCalendarEventTestData() {
    std::vector<BlpConn::HeadlineCalendarEvent> events;

    for (int i = 0; i < 25; ++i) {
        BlpConn::HeadlineCalendarEvent event;
        event.id_bb_global = "BBG002SBQ" + std::to_string(800 + i);
        event.parsekyable_des = "CALENDAR_INDEX_" + std::to_string(i);
        event.description = "Calendar Description " + std::to_string(i);
        event.event_type = static_cast<BlpConn::EventType>(i % 5);
        event.event_subtype = static_cast<BlpConn::EventSubType>(i % 5);
        event.event_id = 2000 + i;
        event.observation_period = "Calendar Period " + std::to_string(i);
        event.release_start_dt = {static_cast<uint64_t>(1773941400000000 + i * 1000), static_cast<short>(i % 24)};
        event.release_end_dt = {static_cast<uint64_t>(1773941400000000 + i * 2000), static_cast<short>(i % 24)};
        event.release_status = static_cast<BlpConn::ReleaseStatus>(i % 4);
        events.push_back(event);
    }

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
