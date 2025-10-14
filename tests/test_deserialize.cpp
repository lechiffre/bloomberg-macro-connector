#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include <vector>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <blpconn_observer.h>
#include "blpconn_fb_generated.h"
#include "blpconn_deserialize.h"

const char* datadir = "./data";

using namespace BlpConn;

std::vector<uint8_t> readFBFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::binary);
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> fileBuffer(fileSize);
    file.read(reinterpret_cast<char*>(fileBuffer.data()), fileSize);
    file.close();
    return fileBuffer;
}

TEST(Deserialize, MacroReferenceData) {
    auto buffer = readFBFile("fb_000008.bin");
    flatbuffers::Verifier verifier(buffer.data(), buffer.size());
    EXPECT_TRUE(BlpConn::FB::VerifyMessageVector(verifier, nullptr, nullptr));
    auto main = flatbuffers::GetRoot<BlpConn::FB::Main>(buffer.data());
    EXPECT_TRUE(main->message_type() == BlpConn::FB::Message_MacroReferenceData);
    auto fb_data = main->message_as_MacroReferenceData();
    auto data = toMacroReferenceData(fb_data);
    EXPECT_EQ(data.corr_id, 12);
    EXPECT_EQ(data.id_bb_global, "BBG002SBJ964");
    EXPECT_EQ(data.parsekyable_des, "CATBTOTB Index");
    EXPECT_EQ(data.description, "STCA Canada Merchandise Trade Total Balance SA CAD");
    EXPECT_EQ(data.indx_freq, "Monthly");
    EXPECT_EQ(data.indx_units, "Value");
    EXPECT_EQ(data.country_iso, "CA");
    EXPECT_EQ(data.indx_source, "STCA - Statistics Canada");
    EXPECT_EQ(data.seasonality_transformation, "Value SA");
}

TEST(Deserialize, MacroHeadlineEvent) {
    auto buffer = readFBFile("fb_000014.bin");
    flatbuffers::Verifier verifier(buffer.data(), buffer.size());
    EXPECT_TRUE(BlpConn::FB::VerifyMessageVector(verifier, nullptr, nullptr));
    auto main = flatbuffers::GetRoot<BlpConn::FB::Main>(buffer.data());
    EXPECT_TRUE(main->message_type() == BlpConn::FB::Message_MacroHeadlineEvent);
    auto fb_data = main->message_as_MacroHeadlineEvent();
    auto data = toMacroHeadlineEvent(fb_data);
    EXPECT_EQ(data.corr_id, 12);
    EXPECT_EQ(data.event_type, EventType::Revision);
    EXPECT_EQ(data.event_subtype, EventSubType::Unitpaint);
    EXPECT_EQ(data.event_id, 2167802);
    EXPECT_EQ(data.observation_period, "Aug");
    EXPECT_EQ(data.release_start_dt.microseconds, 1759840200000000);
    EXPECT_EQ(data.release_start_dt.offset, 0);
    EXPECT_EQ(data.release_end_dt.microseconds, 1759840200000000);
    EXPECT_EQ(data.release_end_dt.offset, 0);
    EXPECT_EQ(data.prior_event_id, 2167801);
    EXPECT_EQ(data.prior_observation_period, "Jul");
    EXPECT_EQ(data.prior_economic_release_start_dt.microseconds, 1756989000000000);
    EXPECT_EQ(data.prior_economic_release_start_dt.offset, 0);
    EXPECT_EQ(data.prior_economic_release_end_dt.microseconds, 1756989000000000);
    EXPECT_EQ(data.prior_economic_release_end_dt.offset, 0);
    EXPECT_EQ(data.value.number, 1);
    EXPECT_DOUBLE_EQ(data.value.value, -3.82);
    EXPECT_TRUE(std::isnan(data.value.low));
    EXPECT_TRUE(std::isnan(data.value.high));
    EXPECT_TRUE(std::isnan(data.value.median));
    EXPECT_TRUE(std::isnan(data.value.average));
    EXPECT_TRUE(std::isnan(data.value.standard_deviation));
}

TEST(Deserialize, MacroCalendarEvent) {
    auto buffer = readFBFile("fb_000012.bin");
    flatbuffers::Verifier verifier(buffer.data(), buffer.size());
    EXPECT_TRUE(BlpConn::FB::VerifyMessageVector(verifier, nullptr, nullptr));
    auto main = flatbuffers::GetRoot<BlpConn::FB::Main>(buffer.data());
    EXPECT_TRUE(main->message_type() == BlpConn::FB::Message_MacroCalendarEvent);
    auto fb_data = main->message_as_MacroCalendarEvent();
    auto data = toMacroCalendarEvent(fb_data);
    EXPECT_EQ(data.corr_id, 12);
    EXPECT_EQ(data.id_bb_global, "");
    EXPECT_EQ(data.parsekyable_des, "");
    EXPECT_EQ(data.event_type, EventType::Calendar);
    EXPECT_EQ(data.event_subtype, EventSubType::Unitpaint);
    EXPECT_EQ(data.description, "");
    EXPECT_EQ(data.event_id, 2167806);
    EXPECT_EQ(data.observation_period, "Dec");
    EXPECT_EQ(data.release_start_dt.microseconds, 1770298200000000);
    EXPECT_EQ(data.release_start_dt.offset, 0);
    EXPECT_EQ(data.release_end_dt.microseconds, 1770298200000000);
    EXPECT_EQ(data.release_end_dt.offset, 0);
    EXPECT_EQ(data.release_status, ReleaseStatus::Scheduled);
    EXPECT_DOUBLE_EQ(data.relevance_value, 55.2632);
}

int main(int argc, char **argv) {
    if (chdir(datadir) == -1) {
        throw std::runtime_error("Error changing directory to debug directory");
    }
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

