#include "blpconn.h"

extern std::string currentTimeStamp();
extern uint64_t currentTime();

json newLogMessage(const std::string& module_name) {
    json j;
    j["module"] = module_name;
    j["timens"] = currentTime();
    return j;
}

std::string jsonToLine(const std::string& message) {
    try {
        std::ostringstream oss;
        json j = json::parse(message);
        for (const auto& item : j.items()) {
            oss << item.key() << "=" << item.value() << " ";
        }
        return oss.str();
    } catch (json::parse_error& e) {
        return message;
    }
}

namespace BlpConn {

void Logger::addNotificationHandler(observerFunc fnc) noexcept {
    callbacks_.push_back(fnc);
}

void Logger::log(const std::string& json_message) {
    if (callbacks_.size() == 0) {
        *out_stream_ << currentTimeStamp() << " " << jsonToLine(json_message) << std::endl;
    } else {
        for (observerFunc fnc : callbacks_) {
            fnc(json_message.c_str());
        }
    } 
}

}



