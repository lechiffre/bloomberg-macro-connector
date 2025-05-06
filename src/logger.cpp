#include <iostream>
#include <string>
#include "blpconn.h"
#include "blpconn_message.h"

namespace BlpConn {

void Logger::addNotificationHandler(ObserverFunc fnc) noexcept {
    callbacks_.push_back(fnc);
}

void Logger::notify(const uint8_t* buffer, size_t size) {
    for (const auto& callback : callbacks_) {
        std::cout << ">>> Calling Callback <<<<" << std::endl;
        callback(buffer, size);
    }
}

void Logger::log(const std::string& module_name, const std::string& message) {
    LogMessage log_message;
    log_message.log_dt = currentTime();
    log_message.module_name = module_name;
    log_message.message = message;
    std::cout << log_message << std::endl;
    if (!out_stream_) {
        *out_stream_ << log_message << std::endl;
    }
    flatbuffers::FlatBufferBuilder builder = buildBufferLogMessage(log_message);
    uint8_t * buffer = builder.GetBufferPointer();
    int size = builder.GetSize();
    notify(buffer, size);
}

} // namespace BlpConn
