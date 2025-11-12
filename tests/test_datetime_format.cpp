#include <gtest/gtest.h>
#include "blpconn_deserialize.h"
#include "blpconn_message.h"
#include <string>

using namespace BlpConn;

class DateTimeFormatTest : public ::testing::Test {
protected:
    // Helper to create a DateTimeType
    DateTimeType createDateTime(uint64_t microseconds, int16_t offset) {
        DateTimeType dt;
        dt.microseconds = microseconds;
        dt.offset = offset;
        return dt;
    }
};

TEST_F(DateTimeFormatTest, ValidDateTimeUTC) {
    // 2025-10-07 12:30:00 UTC
    DateTimeType dt = createDateTime(1759840200000000ULL, 0);
    
    EXPECT_TRUE(isValidDateTime(dt.microseconds));
    
    std::string formatted = formatDateTime(dt);
    EXPECT_NE(formatted, "N/A");
    EXPECT_NE(formatted.find("2025"), std::string::npos);
    EXPECT_NE(formatted.find("UTC"), std::string::npos);
}

TEST_F(DateTimeFormatTest, ValidDateTimeWithMicroseconds) {
    // 2023-10-15 14:30:45.123456 UTC
    DateTimeType dt = createDateTime(1697380245123456ULL, 0);
    
    std::string formatted = formatDateTime(dt);
    EXPECT_NE(formatted, "N/A");
    EXPECT_NE(formatted.find("2023"), std::string::npos);
    EXPECT_NE(formatted.find(".123456"), std::string::npos);
}

TEST_F(DateTimeFormatTest, ValidDateTimeWithPositiveOffset) {
    // 2023-10-15 14:30:45 UTC+2 (120 minutes)
    DateTimeType dt = createDateTime(1697380245000000ULL, 120);
    
    std::string formatted = formatDateTime(dt);
    EXPECT_NE(formatted, "N/A");
    EXPECT_NE(formatted.find("UTC+2"), std::string::npos);
}

TEST_F(DateTimeFormatTest, ValidDateTimeWithNegativeOffset) {
    // 2021-01-01 00:00:00 UTC-5 (−300 minutes)
    DateTimeType dt = createDateTime(1609459200000000ULL, -300);
    
    std::string formatted = formatDateTime(dt);
    EXPECT_NE(formatted, "N/A");
    EXPECT_NE(formatted.find("UTC-5"), std::string::npos);
}

TEST_F(DateTimeFormatTest, FormatDateTimeUTC) {
    DateTimeType dt = createDateTime(1759840200000000ULL, 120);
    
    std::string formatted = formatDateTimeUTC(dt);
    EXPECT_NE(formatted, "N/A");
    EXPECT_NE(formatted.find("2025"), std::string::npos);
    EXPECT_NE(formatted.find("UTC"), std::string::npos);
    // Should always show UTC regardless of offset
    EXPECT_EQ(formatted.find("UTC+"), std::string::npos);
    EXPECT_EQ(formatted.find("UTC-"), std::string::npos);
}

TEST_F(DateTimeFormatTest, FormatDateTimeISO) {
    // 2025-10-07 12:30:00 UTC
    DateTimeType dt = createDateTime(1759840200000000ULL, 0);
    
    std::string formatted = formatDateTimeISO(dt);
    EXPECT_NE(formatted, "N/A");
    EXPECT_NE(formatted.find("2025-10-07T"), std::string::npos);
    EXPECT_NE(formatted.find("Z"), std::string::npos);  // UTC should end with 'Z'
}

TEST_F(DateTimeFormatTest, FormatDateTimeISOWithOffset) {
    // UTC+2 (120 minutes)
    DateTimeType dt = createDateTime(1697380245000000ULL, 120);
    
    std::string formatted = formatDateTimeISO(dt);
    EXPECT_NE(formatted, "N/A");
    EXPECT_NE(formatted.find("T"), std::string::npos);
    EXPECT_NE(formatted.find("+02:00"), std::string::npos);
}

TEST_F(DateTimeFormatTest, FormatDateTimeISOWithNegativeOffset) {
    // UTC-5 (-300 minutes)
    DateTimeType dt = createDateTime(1609459200000000ULL, -300);
    
    std::string formatted = formatDateTimeISO(dt);
    EXPECT_NE(formatted, "N/A");
    EXPECT_NE(formatted.find("-05:00"), std::string::npos);
}

TEST_F(DateTimeFormatTest, UnsetDateTimeZero) {
    DateTimeType dt = createDateTime(0, 0);
    
    EXPECT_FALSE(isValidDateTime(dt.microseconds));
    EXPECT_EQ(formatDateTime(dt), "N/A");
    EXPECT_EQ(formatDateTimeUTC(dt), "N/A");
    EXPECT_EQ(formatDateTimeISO(dt), "N/A");
}

TEST_F(DateTimeFormatTest, InvalidDateTimeOverflow) {
    // Overflow value from negative time_t
    DateTimeType dt = createDateTime(18444534998509551616ULL, 0);
    
    EXPECT_FALSE(isValidDateTime(dt.microseconds));
    EXPECT_EQ(formatDateTime(dt), "N/A");
    EXPECT_EQ(formatDateTimeUTC(dt), "N/A");
    EXPECT_EQ(formatDateTimeISO(dt), "N/A");
}

TEST_F(DateTimeFormatTest, InvalidDateTimeFuture) {
    // Year 5000 - beyond our max valid threshold
    DateTimeType dt = createDateTime(95617584000000000ULL, 0);
    
    EXPECT_FALSE(isValidDateTime(dt.microseconds));
    EXPECT_EQ(formatDateTime(dt), "N/A");
}

TEST_F(DateTimeFormatTest, ConvenienceFunction) {
    // Test the convenience function that takes microseconds and offset directly
    std::string formatted = formatDateTime(1759840200000000ULL, 0);
    
    EXPECT_NE(formatted, "N/A");
    EXPECT_NE(formatted.find("2025"), std::string::npos);
    EXPECT_NE(formatted.find("UTC"), std::string::npos);
}

TEST_F(DateTimeFormatTest, LeapYear) {
    // 2016-02-29 12:00:00 UTC (leap year)
    DateTimeType dt = createDateTime(1456747200000000ULL, 0);
    
    std::string formatted = formatDateTime(dt);
    EXPECT_NE(formatted, "N/A");
    EXPECT_NE(formatted.find("2016"), std::string::npos);
}

TEST_F(DateTimeFormatTest, UnixEpoch) {
    // 1970-01-01 00:00:00 UTC
    DateTimeType dt = createDateTime(0, 0);
    
    // Note: 0 is treated as unset, not Unix epoch
    EXPECT_EQ(formatDateTime(dt), "N/A");
}

TEST_F(DateTimeFormatTest, Year2000) {
    // 2000-01-01 00:00:00 UTC
    DateTimeType dt = createDateTime(946684800000000ULL, 0);
    
    std::string formatted = formatDateTime(dt);
    EXPECT_NE(formatted, "N/A");
    EXPECT_NE(formatted.find("2000"), std::string::npos);
}

TEST_F(DateTimeFormatTest, CurrentTime) {
    DateTimeType now = currentTime();
    
    EXPECT_TRUE(isValidDateTime(now.microseconds));
    
    std::string formatted = formatDateTime(now);
    EXPECT_NE(formatted, "N/A");
    
    std::string iso = formatDateTimeISO(now);
    EXPECT_NE(iso, "N/A");
}

TEST_F(DateTimeFormatTest, MicrosecondsPreserved) {
    // Test that microseconds are properly displayed
    DateTimeType dt = createDateTime(1609459200123456ULL, 0);
    
    std::string formatted = formatDateTime(dt);
    EXPECT_NE(formatted.find(".123456"), std::string::npos);
    
    std::string iso = formatDateTimeISO(dt);
    EXPECT_NE(iso.find(".123456"), std::string::npos);
}

TEST_F(DateTimeFormatTest, NoMicrosecondsWhenZero) {
    // Test that .000000 is not displayed when microseconds are 0
    DateTimeType dt = createDateTime(1609459200000000ULL, 0);
    
    std::string formatted = formatDateTime(dt);
    EXPECT_EQ(formatted.find(".000000"), std::string::npos);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
