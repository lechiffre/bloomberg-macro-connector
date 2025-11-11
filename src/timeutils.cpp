#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <blpconn_message.h>

namespace BlpConn {

// Maximum valid microseconds (year 3000-01-01 00:00:00 UTC)
static const uint64_t MAX_VALID_MICROSECONDS = 32503680000000000ULL;

// Check if a datetime is valid/set
bool isValidDateTime(uint64_t microseconds) {
    return microseconds > 0 && microseconds < MAX_VALID_MICROSECONDS;
}

// Convert DateTimeType to human-readable string in UTC
std::string formatDateTimeUTC(const DateTimeType& dt) {
    if (!isValidDateTime(dt.microseconds)) {
        return "N/A";
    }
    
    std::time_t seconds = dt.microseconds / 1000000;
    uint64_t micros = dt.microseconds % 1000000;
    std::tm* tm = std::gmtime(&seconds);
    
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    if (micros > 0) {
        oss << "." << std::setfill('0') << std::setw(6) << micros;
    }
    oss << " UTC";
    
    return oss.str();
}

// Convert DateTimeType to human-readable string with timezone offset
std::string formatDateTime(const DateTimeType& dt) {
    if (!isValidDateTime(dt.microseconds)) {
        return "N/A";
    }
    
    std::time_t seconds = dt.microseconds / 1000000;
    uint64_t micros = dt.microseconds % 1000000;
    std::tm* tm = std::gmtime(&seconds);
    
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%d %H:%M:%S");
    if (micros > 0) {
        oss << "." << std::setfill('0') << std::setw(6) << micros;
    }
    
    // Format timezone offset
    if (dt.offset == 0) {
        oss << " UTC";
    } else {
        int hours = dt.offset / 60;
        int mins = std::abs(dt.offset % 60);
        oss << " UTC" << std::showpos << hours;
        if (mins > 0) {
            oss << std::noshowpos << ":" << std::setfill('0') << std::setw(2) << mins;
        }
    }
    
    return oss.str();
}

// Convert DateTimeType to ISO 8601 format (YYYY-MM-DDTHH:MM:SS.ssssss+HH:MM)
std::string formatDateTimeISO(const DateTimeType& dt) {
    if (!isValidDateTime(dt.microseconds)) {
        return "N/A";
    }
    
    std::time_t seconds = dt.microseconds / 1000000;
    uint64_t micros = dt.microseconds % 1000000;
    std::tm* tm = std::gmtime(&seconds);
    
    std::ostringstream oss;
    oss << std::put_time(tm, "%Y-%m-%dT%H:%M:%S");
    if (micros > 0) {
        oss << "." << std::setfill('0') << std::setw(6) << micros;
    }
    
    // Format timezone offset in ISO format
    if (dt.offset == 0) {
        oss << "Z";
    } else {
        int hours = dt.offset / 60;
        int mins = std::abs(dt.offset % 60);
        // Manually format with sign and padding
        oss << (hours >= 0 ? "+" : "-")
            << std::setfill('0') << std::setw(2) << std::abs(hours)
            << ":" << std::setfill('0') << std::setw(2) << mins;
    }
    
    return oss.str();
}

// Convert microseconds and offset to human-readable string (convenience function)
std::string formatDateTime(uint64_t microseconds, int16_t offset) {
    DateTimeType dt;
    dt.microseconds = microseconds;
    dt.offset = offset;
    return formatDateTime(dt);
}

DateTimeType currentTime() noexcept {
    DateTimeType dt;
    auto now = std::chrono::system_clock::now();
    auto since_epoch = now.time_since_epoch();
    auto microseconds = std::chrono::duration_cast<std::chrono::microseconds>(since_epoch);
    dt.microseconds = microseconds.count();
    
    // Calculate the offset
    std::time_t now_c = std::chrono::system_clock::to_time_t(now);
    std::tm local_tm = *std::localtime(&now_c);
    std::tm utc_tm = *std::gmtime(&now_c);
    dt.offset = static_cast<short>(std::difftime(std::mktime(&local_tm), std::mktime(&utc_tm)) / 60);

    return dt;
}

std::string currentTimeStamp() {
    time_t now = std::time(nullptr);
    std::ostringstream oss;
    oss << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

} // namespace BlpConn

uint64_t currentTime() {
    auto time = std::chrono::system_clock::now();
    auto since_epoch = time.time_since_epoch();
    auto milis = std::chrono::duration_cast<std::chrono::nanoseconds>(since_epoch);
    return milis.count();
}

