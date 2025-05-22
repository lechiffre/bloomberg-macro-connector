#include <gtest/gtest.h>
#include <blpapi_datetime.h>
#include <ctime>
#include "../src/parser.cpp" // For toMicrosecondsSinceEpoch

using namespace BloombergLP;

namespace {

TEST(ToMicrosecondsSinceEpochTest, UnixEpoch) {
    blpapi::Datetime dt;
    dt.setDate(1970, 1, 1);
    dt.setTime(0, 0, 0, 0);
    uint64_t micros = BlpConn::toMicrosecondsSinceEpoch(dt);
    std::tm tm = {};
    tm.tm_year = 1970 - 1900;
    tm.tm_mon = 0;
    tm.tm_mday = 1;
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;
#if defined(_GNU_SOURCE) || defined(__APPLE__)
    std::time_t t = timegm(&tm);
#else
    std::time_t t = std::mktime(&tm);
#endif
    uint64_t expected = static_cast<uint64_t>(t) * 1000000;
    EXPECT_EQ(micros, expected);
}

TEST(ToMicrosecondsSinceEpochTest, KnownDate) {
    blpapi::Datetime dt;
    dt.setDate(2000, 1, 2);
    dt.setTime(3, 4, 5, 123456);
    dt.setOffset(0);
    uint64_t micros = BlpConn::toMicrosecondsSinceEpoch(dt);

    std::tm tm = {};
    tm.tm_year = 2000 - 1900;
    tm.tm_mon = 0;
    tm.tm_mday = 2;
    tm.tm_hour = 3;
    tm.tm_min = 4;
    tm.tm_sec = 5;
#if defined(_GNU_SOURCE) || defined(__APPLE__)
    std::time_t t = timegm(&tm);
#else
    std::time_t t = std::mktime(&tm);
#endif
    uint64_t expected = static_cast<uint64_t>(t) * 1000000 + 123456 * 1000;
    EXPECT_EQ(micros, expected);
}

TEST(ToMicrosecondsSinceEpochTest, LeapYear) {
    blpapi::Datetime dt;
    dt.setDate(2016, 2, 29);
    dt.setTime(12, 0, 0, 0);
    uint64_t micros = BlpConn::toMicrosecondsSinceEpoch(dt);

    std::tm tm = {};
    tm.tm_year = 2016 - 1900;
    tm.tm_mon = 1;
    tm.tm_mday = 29;
    tm.tm_hour = 12;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    std::time_t t = std::mktime(&tm);
    uint64_t expected = static_cast<uint64_t>(t) * 1000000;
    EXPECT_EQ(micros, expected);
}

} // namespace

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
