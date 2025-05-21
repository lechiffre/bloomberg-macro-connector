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
#ifdef DEBUG
    uint64_t event_id = 0;
    if (testing_) {
        event_id = profiler_.push("Logger", "notify", "NotifyObservers");
    }
#endif
    for (const auto& callback : callbacks_) {
        callback(buffer, size);
    }
#ifdef DEBUG
    if (testing_) {
        profiler_.push("Logger", "notify", "NotifyObservers", event_id);
    }
#endif
}

void Logger::log(uint8_t module, uint8_t status, uint64_t correlation_id,
    const std::string& message)
{
    if (module == 0) return;
#ifdef DEBUG
    uint64_t event_id = 0;
    if (testing_) {
        event_id = profiler_.push("Logger", "log", "LogMessage");
    }
#endif
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

    // Only print to std::cout if in debug mode
#ifdef DEBUG
    std::cout << log_message << std::endl;
#endif

    // Build FlatBuffer and notify observers
    flatbuffers::FlatBufferBuilder builder = buildBufferLogMessage(log_message);
    notify(builder.GetBufferPointer(), builder.GetSize());

#ifdef DEBUG
    if (testing_) {
        profiler_.push("Logger", "log", "LogMessage", event_id);
    }
#endif
}

} // namespace BlpConn
