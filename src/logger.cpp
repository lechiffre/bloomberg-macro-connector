#include <iostream>
#include <string>
#include "blpconn_logger.h"
#include "blpconn_message.h"
#include "blpconn_deserialize.h"

namespace BlpConn {

void Logger::addNotificationHandler(ObserverFunc fnc) noexcept {
    callbacks_.push_back(fnc);
}

void Logger::notify(const uint8_t* buffer, size_t size) {
    for (const auto& callback : callbacks_) {
        callback(buffer, size);
    }
}

void Logger::log(uint8_t  module, uint8_t status, uint64_t correlation_id,
    const std::string& message)
{
    if (module == 0) return;
    LogMessage log_message;
    log_message.log_dt = currentTime();
    log_message.module = module;
    log_message.status = status;
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
