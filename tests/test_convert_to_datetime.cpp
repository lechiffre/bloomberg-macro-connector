#include <blpapi_datetime.h>
#include <gtest/gtest.h>
#include <ctime>
#include "blpconn_serialize.h"

using namespace BloombergLP;
using namespace BlpConn;

class ConvertToDateTimeTest : public ::testing::Test {
protected:
    static std::tm createTimeStruct(int year, int month, int day, int hour, int minute, int second) {
        std::tm timeStruct = {};
        timeStruct.tm_year = year - 1900;
        timeStruct.tm_mon = month - 1;
        timeStruct.tm_mday = day;
        timeStruct.tm_hour = hour;
        timeStruct.tm_min = minute;
        timeStruct.tm_sec = second;
        return timeStruct;
    }
};

TEST_F(ConvertToDateTimeTest, ValidFullDatetime) {
    blpapi::Datetime blpDatetime(2023, 10, 15, 14, 30, 45, blpapi::Datetime::Microseconds(123456));
    blpDatetime.setOffset(120); // UTC+2

    BlpConn::DateTimeType dateTime = BlpConn::convertToDateTime(blpDatetime);

    std::tm timeStruct = createTimeStruct(2023, 10, 15, 14, 30, 45);
    std::time_t expectedTimeSinceEpoch = std::mktime(&timeStruct);
    uint64_t expectedMicros = static_cast<uint64_t>(expectedTimeSinceEpoch) * 1000000 + 123456;

    EXPECT_EQ(dateTime.microseconds, expectedMicros);
    EXPECT_EQ(dateTime.offset, 120);
}

TEST_F(ConvertToDateTimeTest, ValidMinimalDatetime) {
    blpapi::Datetime blpDatetime(2023, 1, 1, 0, 0, 0, blpapi::Datetime::Microseconds(0));
    blpDatetime.setOffset(0); // UTC

    BlpConn::DateTimeType dateTime = convertToDateTime(blpDatetime);

    std::tm timeStruct = createTimeStruct(2023, 1, 1, 0, 0, 0);
    std::time_t expectedTimeSinceEpoch = std::mktime(&timeStruct);
    uint64_t expectedMicros = static_cast<uint64_t>(expectedTimeSinceEpoch) * 1000000;

    EXPECT_EQ(dateTime.microseconds, expectedMicros);
    EXPECT_EQ(dateTime.offset, 0);
}

TEST_F(ConvertToDateTimeTest, InvalidUnsetYear) {
    blpapi::Datetime blpDatetime(0, 10, 15, 14, 30, 45, blpapi::Datetime::Microseconds(123456));
    blpDatetime.setOffset(120);

    EXPECT_THROW(convertToDateTime(blpDatetime), std::exception);
}

TEST_F(ConvertToDateTimeTest, InvalidMonthOutOfRange) {
    blpapi::Datetime blpDatetime(2023, 13, 15, 14, 30, 45, blpapi::Datetime::Microseconds(123456)); // Invalid month
    blpDatetime.setOffset(120);

    EXPECT_THROW(convertToDateTime(blpDatetime), std::exception);
}

TEST_F(ConvertToDateTimeTest, InvalidDayOutOfRange) {
    blpapi::Datetime blpDatetime(2023, 2, 30, 14, 30, 45, blpapi::Datetime::Microseconds(123456)); // Invalid day
    blpDatetime.setOffset(120);

    EXPECT_THROW(convertToDateTime(blpDatetime), std::exception);
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
