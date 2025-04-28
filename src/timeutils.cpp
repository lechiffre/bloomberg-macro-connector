#include <ctime>
#include <chrono>
#include <iomanip>
#include <sstream>

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

