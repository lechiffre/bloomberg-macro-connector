#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <blpconn_message.h>
#include <blpconn_serialize.h>

namespace BlpConn {
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
}

std::string currentTimeStamp() {
      time_t now = std::time(nullptr);
      std::ostringstream oss;
      oss << std::put_time(std::localtime(&now), "%Y-%m-%d %H:%M:%S");
      return oss.str();
}

uint64_t currentTime() {
     auto time = std::chrono::system_clock::now();
     auto since_epoch = time.time_since_epoch();
     auto milis = std::chrono::duration_cast<std::chrono::nanoseconds>(since_epoch);
     return milis.count();
}

