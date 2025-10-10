#include <vector>
#include <gtest/gtest.h>
#include <flatbuffers/flatbuffers.h>
#include <blpconn_serialize.h>
#include <blpconn_deserialize.h>

using namespace BlpConn;

/**
 * Return three examples of MacroReferenceData:
 * 1. Valid data
 * 2. Missing id_bb_global (failure case)
 * 3. Missing parsekyable_des (failure case)
 */
std::vector<MacroReferenceData> createMacroReferenceDataTestData() {
    std::vector<MacroReferenceData> data;

    // Valid case #1
    MacroReferenceData valid_data;
    valid_data.corr_id = 10;
    valid_data.id_bb_global = "BBG002SBJ900";
    valid_data.parsekyable_des = "TEST_INDEX_0";
    valid_data.description = "Valid Description";
    valid_data.indx_freq = "Monthly";
    valid_data.indx_units = "Percent";
    valid_data.country_iso = "US";
    valid_data.indx_source = "Valid Source";
    valid_data.seasonality_transformation = "Seasonally Adjusted";
    data.push_back(valid_data);

    // Valid case #2
    valid_data.corr_id = 11;
    valid_data.id_bb_global = "BBG002SBK123";
    valid_data.parsekyable_des = "TEST_INDEX_1";
    valid_data.description = "Another Valid Description";
    valid_data.indx_freq = "Quarterly";
    valid_data.indx_units = "Index";
    valid_data.country_iso = "GB";
    valid_data.indx_source = "Another Source";
    valid_data.seasonality_transformation = "Not Seasonally Adjusted";
    data.push_back(valid_data);

    // Valid case #3
    valid_data.corr_id = 12;
    valid_data.id_bb_global = "BBG002SBX456";
    valid_data.parsekyable_des = "TEST_INDEX_2";
    valid_data.description = "Yet Another Valid Description";
    valid_data.indx_freq = "Yearly";
    valid_data.indx_units = "Points";
    valid_data.country_iso = "JP";
    valid_data.indx_source = "Different Source";
    valid_data.seasonality_transformation = "Seasonally Adjusted";
    data.push_back(valid_data);

    return data;
}

std::vector<MacroHeadlineEvent> createMacroHeadlineEventTestData() {
    std::vector<MacroHeadlineEvent> events;

    // Valid case #1
    MacroHeadlineEvent valid_event;
    valid_event.corr_id = 11;
    valid_event.event_type = EventType::Actual;
    valid_event.event_subtype = EventSubType::New;
    valid_event.event_id = 1000;
    valid_event.observation_period = "Valid Period";
    valid_event.release_start_dt = {1743701400000000, 0};
    valid_event.release_end_dt = {1743701400000000, 0};
    valid_event.prior_event_id = 999;
    valid_event.prior_observation_period = "Prior Period";
    valid_event.prior_economic_release_start_dt = {0, 0};
    valid_event.prior_economic_release_end_dt = {0, 0};
    valid_event.value = {1, -1.52, NAN, NAN, NAN, NAN, NAN};
    events.push_back(valid_event);

    // Valid case #2
    valid_event.corr_id = 12;
    valid_event.event_type = EventType::Revision;
    valid_event.event_subtype = EventSubType::Update;
    valid_event.event_id = 1001;
    valid_event.observation_period = "Forecast Period";
    valid_event.release_start_dt = {1743787800000000, 0};
    valid_event.release_end_dt = {1743787800000000, 0};
    valid_event.prior_event_id = 1000;
    valid_event.prior_observation_period = "Valid Period";
    valid_event.prior_economic_release_start_dt = {1743701400000000, 0};
    valid_event.prior_economic_release_end_dt = {1743701400000000, 0};
    valid_event.value = {1, 2.34, NAN, NAN, NAN, NAN, NAN};
    events.push_back(valid_event);

    // Valid case #3
    valid_event.corr_id = 13;
    valid_event.event_type = EventType::Calendar;
    valid_event.event_subtype = EventSubType::Unitpaint;
    valid_event.event_id = 1002;
    valid_event.observation_period = "Estimate Period";
    valid_event.release_start_dt = {1743874200000000, 0};
    valid_event.release_end_dt = {1743874200000000, 0};
    valid_event.prior_event_id = 1001;
    valid_event.prior_observation_period = "Forecast Period";
    valid_event.prior_economic_release_start_dt = {1743787800000000, 0};
    valid_event.prior_economic_release_end_dt = {1743787800000000, 0};
    valid_event.value = {1, 3.45, NAN, NAN, NAN, NAN, NAN};
    events.push_back(valid_event);

    return events;
}

std::vector<MacroCalendarEvent> createMacroCalendarEventTestData() {
    std::vector<MacroCalendarEvent> events;

    // Valid case #1
    MacroCalendarEvent valid_event;
    valid_event.corr_id = 11;
    valid_event.id_bb_global = "BBG002SBQ800";
    valid_event.parsekyable_des = "CALENDAR_INDEX_0";
    valid_event.event_type = EventType::Calendar;
    valid_event.event_subtype = EventSubType::Update;
    valid_event.description = "Valid Calendar Description";
    valid_event.event_id = 2000;
    valid_event.observation_period = "Calendar Period";
    valid_event.release_start_dt = {1773941400000000, 0};
    valid_event.release_end_dt = {1773941400000000, 0};
    valid_event.release_status = ReleaseStatus::Scheduled;
    valid_event.relevance_value = 0.85;
    events.push_back(valid_event);
    
    // Valid case #2
    valid_event.corr_id = 12;
    valid_event.id_bb_global = "BBG002SBQ801";
    valid_event.parsekyable_des = "CALENDAR_INDEX_1";
    valid_event.event_type = EventType::Calendar;
    valid_event.event_subtype = EventSubType::New;
    valid_event.description = "Another Calendar Description";
    valid_event.event_id = 2001;
    valid_event.observation_period = "Another Calendar Period";
    valid_event.release_start_dt = {1774027800000000, 0};
    valid_event.release_end_dt = {1774027800000000, 0};
    valid_event.release_status = ReleaseStatus::Released;
    valid_event.relevance_value = 0.90;
    events.push_back(valid_event);

    // Valid case #3
    valid_event.corr_id = 13;
    valid_event.id_bb_global = "BBG002SBQ802";
    valid_event.parsekyable_des = "CALENDAR_INDEX_2";
    valid_event.event_type = EventType::Calendar;
    valid_event.event_subtype = EventSubType::Delete;
    valid_event.description = "Yet Another Calendar Description";
    valid_event.event_id = 2002;
    valid_event.observation_period = "Yet Another Calendar Period";
    valid_event.release_start_dt = {1774114200000000, 0};
    valid_event.release_end_dt = {1774114200000000, 0};
    valid_event.release_status = ReleaseStatus::Scheduled;
    valid_event.relevance_value = 0.75;
    events.push_back(valid_event);

    return events;
}

class MacroReferenceTest: public ::testing::TestWithParam<MacroReferenceData> {};

TEST_P(MacroReferenceTest, MacroReferenceData) {
    const auto& original_data = GetParam();

    flatbuffers::FlatBufferBuilder builder;
    auto serialized_data = serializeMacroReferenceData(builder, 10, original_data);
    builder.Finish(serialized_data);

    // Deserialize the object
    auto fb_data = flatbuffers::GetRoot<FB::MacroReferenceData>(builder.GetBufferPointer());
    MacroReferenceData deserialized_data = toMacroReferenceData(fb_data);

    // Validate the deserialized object
    EXPECT_EQ(deserialized_data.id_bb_global, original_data.id_bb_global);
    EXPECT_EQ(deserialized_data.parsekyable_des, original_data.parsekyable_des);
    EXPECT_EQ(deserialized_data.description, original_data.description);
    EXPECT_EQ(deserialized_data.indx_freq, original_data.indx_freq);
    EXPECT_EQ(deserialized_data.indx_units, original_data.indx_units);
    EXPECT_EQ(deserialized_data.country_iso, original_data.country_iso);
    EXPECT_EQ(deserialized_data.indx_source, original_data.indx_source);
    EXPECT_EQ(deserialized_data.seasonality_transformation, original_data.seasonality_transformation);
}

class MacroHeadlineEventTest: public ::testing::TestWithParam<MacroHeadlineEvent> {};

TEST_P(MacroHeadlineEventTest, MacroHeadlineEvent) {
    const auto& original_event = GetParam();
    // Serialize the object
    flatbuffers::FlatBufferBuilder builder;
    auto serialized_event = serializeMacroHeadlineEvent(builder, 10, original_event);
    builder.Finish(serialized_event);

    // Deserialize the object
    auto fb_event = flatbuffers::GetRoot<FB::MacroHeadlineEvent>(builder.GetBufferPointer());
    MacroHeadlineEvent deserialized_event = toMacroHeadlineEvent(fb_event);

    // Validate the deserialized object
    EXPECT_EQ(deserialized_event.event_type, original_event.event_type);
    EXPECT_EQ(deserialized_event.event_subtype, original_event.event_subtype);
    EXPECT_EQ(deserialized_event.event_id, original_event.event_id);
    EXPECT_EQ(deserialized_event.observation_period, original_event.observation_period);
    EXPECT_EQ(deserialized_event.release_start_dt.microseconds, original_event.release_start_dt.microseconds);
    EXPECT_EQ(deserialized_event.release_start_dt.offset, original_event.release_start_dt.offset);
    EXPECT_EQ(deserialized_event.release_end_dt.microseconds, original_event.release_end_dt.microseconds);
    EXPECT_EQ(deserialized_event.release_end_dt.offset, original_event.release_end_dt.offset);
    EXPECT_EQ(deserialized_event.prior_event_id, original_event.prior_event_id);
    EXPECT_EQ(deserialized_event.prior_observation_period, original_event.prior_observation_period);
    EXPECT_EQ(deserialized_event.prior_economic_release_start_dt.microseconds, original_event.prior_economic_release_start_dt.microseconds);
    EXPECT_EQ(deserialized_event.prior_economic_release_start_dt.offset, original_event.prior_economic_release_start_dt.offset);
    EXPECT_EQ(deserialized_event.prior_economic_release_end_dt.microseconds, original_event.prior_economic_release_end_dt.microseconds);
    EXPECT_EQ(deserialized_event.prior_economic_release_end_dt.offset, original_event.prior_economic_release_end_dt.offset);
    EXPECT_EQ(deserialized_event.value.number, original_event.value.number);
    EXPECT_NEAR(deserialized_event.value.value, original_event.value.value, 1e-6);
}

class MacroCalendarEventTest: public ::testing::TestWithParam<MacroCalendarEvent> {};

TEST_P(MacroCalendarEventTest, MacroCalendarEvent) {
    const auto& original_event = GetParam();

    // Serialize the object
    flatbuffers::FlatBufferBuilder builder;
    auto serialized_event = serializeMacroCalendarEvent(builder, 10, original_event);
    builder.Finish(serialized_event);

    // Deserialize the object
    auto fb_event = flatbuffers::GetRoot<FB::MacroCalendarEvent>(builder.GetBufferPointer());
    MacroCalendarEvent deserialized_event = toMacroCalendarEvent(fb_event);

    // Validate the deserialized object
    EXPECT_EQ(deserialized_event.id_bb_global, original_event.id_bb_global);
    EXPECT_EQ(deserialized_event.parsekyable_des, original_event.parsekyable_des);
    EXPECT_EQ(deserialized_event.event_type, original_event.event_type);
    EXPECT_EQ(deserialized_event.event_subtype, original_event.event_subtype);
    EXPECT_EQ(deserialized_event.description, original_event.description);
    EXPECT_EQ(deserialized_event.event_id, original_event.event_id);
    EXPECT_EQ(deserialized_event.observation_period, original_event.observation_period);
    EXPECT_EQ(deserialized_event.release_start_dt.microseconds, original_event.release_start_dt.microseconds);
    EXPECT_EQ(deserialized_event.release_start_dt.offset, original_event.release_start_dt.offset);
    EXPECT_EQ(deserialized_event.release_end_dt.microseconds, original_event.release_end_dt.microseconds);
    EXPECT_EQ(deserialized_event.release_end_dt.offset, original_event.release_end_dt.offset);
    EXPECT_EQ(deserialized_event.release_status, original_event.release_status);
    EXPECT_NEAR(deserialized_event.relevance_value, original_event.relevance_value, 1e-6);
}

INSTANTIATE_TEST_SUITE_P(MacroReferenceTests, MacroReferenceTest, ::testing::ValuesIn(createMacroReferenceDataTestData()));
INSTANTIATE_TEST_SUITE_P(MacroHeadlineEventTests, MacroHeadlineEventTest, ::testing::ValuesIn(createMacroHeadlineEventTestData()));
INSTANTIATE_TEST_SUITE_P(MacroCalendarEventTests, MacroCalendarEventTest, ::testing::ValuesIn(createMacroCalendarEventTestData()));

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
