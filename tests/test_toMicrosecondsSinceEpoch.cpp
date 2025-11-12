#include <gtest/gtest.h>
#include <blpapi_datetime.h>
#include <ctime>
#include <cstdint>
#include <stdexcept>

using namespace BloombergLP;

// Since toMicrosecondsSinceEpoch is a static function in parser.cpp,
// we need to recreate its logic here for testing
namespace {

static uint64_t toMicrosecondsSinceEpoch(const blpapi::Datetime& blpDatetime) {
    std::tm timeStruct = {};
    std::time_t timeSinceEpoch;
    int64_t micros = 0;
    int orig_year = 0, orig_month = 0, orig_day = 0;
    
    if (blpDatetime.hasParts(blpapi::DatetimeParts::DATE)) {
        int year = blpDatetime.year();
        int month = blpDatetime.month();
        int day = blpDatetime.day();
        
        if (year == 0) {
            throw std::invalid_argument("Invalid datetime: year is not set");
        }
        if (month < 1 || month > 12) {
            throw std::invalid_argument("Invalid datetime: month out of range");
        }
        if (day < 1 || day > 31) {
            throw std::invalid_argument("Invalid datetime: day out of range");
        }
        
        timeStruct.tm_year = year - 1900;
        timeStruct.tm_mon = month - 1;
        timeStruct.tm_mday = day;
        
        orig_year = timeStruct.tm_year;
        orig_month = timeStruct.tm_mon;
        orig_day = timeStruct.tm_mday;
    }
    if (blpDatetime.hasParts(blpapi::DatetimeParts::TIME)) {
        timeStruct.tm_hour = blpDatetime.hours();
        timeStruct.tm_min = blpDatetime.minutes();
        timeStruct.tm_sec = blpDatetime.seconds();
    }
    timeSinceEpoch = timegm(&timeStruct);
    if (timeSinceEpoch == -1) {
        throw std::invalid_argument("Invalid datetime: cannot convert to epoch");
    }
    
    // Verify that timegm didn't normalize the date
    if (blpDatetime.hasParts(blpapi::DatetimeParts::DATE)) {
        std::tm verifyStruct = *gmtime(&timeSinceEpoch);
        if (verifyStruct.tm_year != orig_year ||
            verifyStruct.tm_mon != orig_month ||
            verifyStruct.tm_mday != orig_day) {
            throw std::invalid_argument("Invalid datetime: day does not exist in month");
        }
    }
    
    micros = blpDatetime.hasParts(blpapi::DatetimeParts::FRACSECONDS)
                         ? blpDatetime.microseconds()
                         : 0;
    return static_cast<uint64_t>(timeSinceEpoch) * 1000000 + micros;
}

TEST(ToMicrosecondsSinceEpochTest, UnixEpoch) {
    blpapi::Datetime dt;
    dt.setDate(1970, 1, 1);
    dt.setTime(0, 0, 0, 0);
    uint64_t micros = toMicrosecondsSinceEpoch(dt);
    std::tm tm = {};
    tm.tm_year = 1970 - 1900;
    tm.tm_mon = 0;
    tm.tm_mday = 1;
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    std::time_t t = timegm(&tm);
    uint64_t expected = static_cast<uint64_t>(t) * 1000000;
    EXPECT_EQ(micros, expected);
}

TEST(ToMicrosecondsSinceEpochTest, KnownDate) {
    blpapi::Datetime dt;
    dt.setDate(2000, 1, 2);
    dt.setTime(3, 4, 5, 123);  // Note: setTime takes milliseconds as 4th parameter
    dt.setOffset(0);
    uint64_t micros = toMicrosecondsSinceEpoch(dt);

    std::tm tm = {};
    tm.tm_year = 2000 - 1900;
    tm.tm_mon = 0;
    tm.tm_mday = 2;
    tm.tm_hour = 3;
    tm.tm_min = 4;
    tm.tm_sec = 5;
    std::time_t t = timegm(&tm);
    uint64_t expected = static_cast<uint64_t>(t) * 1000000 + 123000;  // 123 milliseconds = 123000 microseconds
    EXPECT_EQ(micros, expected);
}

TEST(ToMicrosecondsSinceEpochTest, LeapYear) {
    blpapi::Datetime dt;
    dt.setDate(2016, 2, 29);
    dt.setTime(12, 0, 0, 0);
    uint64_t micros = toMicrosecondsSinceEpoch(dt);

    std::tm tm = {};
    tm.tm_year = 2016 - 1900;
    tm.tm_mon = 1;
    tm.tm_mday = 29;
    tm.tm_hour = 12;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    std::time_t t = timegm(&tm);
    uint64_t expected = static_cast<uint64_t>(t) * 1000000;
    EXPECT_EQ(micros, expected);
}

} // namespace

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
