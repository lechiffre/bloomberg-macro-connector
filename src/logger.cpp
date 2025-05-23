#include <iostream>
#include <string>
#include "blpconn_logger.h"
#include "blpconn_message.h"
#include "blpconn_deserialize.h"
#include "blpconn_profiler.h"

namespace BlpConn {

void Logger::addNotificationHandler(ObserverFunc fnc) noexcept {
    callbacks_.push_back(fnc);
}

void Logger::notify(const uint8_t* buffer, size_t size) {
    PROFILE_FUNCTION()
    for (const auto& callback : callbacks_) {
        callback(buffer, size);
    }
    END_PROFILE_FUNCTION()  
}

void Logger::log(uint8_t module, uint8_t status, uint64_t correlation_id,
    const std::string& message)
{
    PROFILE_FUNCTION()
    if (module == 0) return;
    LogMessage log_message;
    log_message.log_dt = currentTime();
    log_message.module = module;
    log_message.status = status;
    log_message.correlation_id = correlation_id;
    log_message.message = message;

    // Avoid unnecessary locking and I/O if not needed
    // Only write to out_stream_ if it is set
    if (out_stream_) {
        *out_stream_ << log_message << std::endl;
    }

    // Build FlatBuffer and notify observers
    flatbuffers::FlatBufferBuilder builder = buildBufferLogMessage(log_message);
    END_PROFILE_FUNCTION()
    notify(builder.GetBufferPointer(), builder.GetSize());
}

} // namespace BlpConn
