#include <iostream>
#include <string>
#include "blpconn.h"

namespace BlpConn {

void Logger::addNotificationHandler(ObserverFunc fnc) noexcept {
    std::cout << ">>> Logger addNotificationHandler" << std::endl;
    callbacks_.push_back(fnc);
}

/*
void Logger::send_notification(Message message, MessageType msg_type) {
    flatbuffers::FlatBufferBuilder builder;
    switch (msg_type) {
        case MessageType::LogMessage: {
            auto fb_log_message = serializeLogMessage(builder, std::get<LogMessage>(message)).Union();
            builder.Finish(fb_log_message);
            break;
        }
        case MessageType::EconomicEvent: {
            try {
                auto fb_economic_event = serializeHeadlineEconomicEvent(builder, std::get<HeadlineEconomicEvent>(message)).Union();
                builder.Finish(fb_economic_event);
            } catch (std::bad_variant_access& e) {
                std::cout << ">>> Error: " << e.what() << std::endl;
            }
            break;
        }
        case MessageType::CalendarEvent: {
            auto fb_calendar_event = serializeHeadlineCalendarEvent(builder, std::get<HeadlineCalendarEvent>(message)).Union();
            builder.Finish(fb_calendar_event);
            break;
        }
    }
    uint8_t * buffer = builder.GetBufferPointer();
    int size = builder.GetSize();
    notify(buffer, size);
}
*/

void Logger::sendNotification(flatbuffers::FlatBufferBuilder& builder) {
    uint8_t * buffer = builder.GetBufferPointer();
    int size = builder.GetSize();
    notify(buffer, size);
}


void Logger::notify(const uint8_t* buffer, size_t size) {

    std::cout << ">>> Logger notify" 
        << callbacks_.size() << std::endl;
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
    if (!out_stream_) {
        *out_stream_ << log_message << std::endl;
    }
    // TODO: Review lifetime of builder
    flatbuffers::FlatBufferBuilder builder;
    auto fb_message = serializeLogMessage(builder, log_message).Union();
    builder.Finish(fb_message);
    uint8_t * buffer = builder.GetBufferPointer();
    int size = builder.GetSize();
    notify(buffer, size);
}

} // namespace BlpConn
