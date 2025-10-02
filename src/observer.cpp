#include "blpconn_observer.h"
#include "blpconn_fb_generated.h"
#include "blpconn_deserialize.h"

namespace BlpConn {

void defaultObserver(const uint8_t *buffer, size_t size) {
    PROFILE_FUNCTION()
    flatbuffers::Verifier verifier(buffer, size);
    if (!BlpConn::FB::VerifyMessageVector(verifier, nullptr, nullptr)) {
        std::cout << "Invalid message" << std::endl;
        return;
    }
    auto main = flatbuffers::GetRoot<BlpConn::FB::Main>(buffer);
    std::cout << "Received message of type: " << main->message_type() << std::endl;
    if (main->message_type() == BlpConn::FB::Message_HeadlineEconomicEvent) {
        auto fb_event = main->message_as_HeadlineEconomicEvent();
        auto event = toHeadlineEconomicEvent(fb_event);
        std::cout << event << std::endl;
    } else if (main->message_type() == FB::Message_HeadlineCalendarEvent) {
        auto fb_event = main->message_as_HeadlineCalendarEvent();
        auto event = toHeadlineCalendarEvent(fb_event);
        std::cout << event << std::endl;
    } else if (main->message_type() == BlpConn::FB::Message_LogMessage) {
        auto fb_log_message = main->message_as_LogMessage();
        auto log_message = toLogMessage(fb_log_message);
        std::cout << log_message << std::endl;
    } else if (main->message_type() == BlpConn::FB::Message_MacroReferenceData) {
        auto fb_data = main->message_as_MacroReferenceData();
        auto data = toMacroReferenceData(fb_data);
        std::cout << data << std::endl;
    } else if (main->message_type() == BlpConn::FB::Message_MacroHeadlineEvent) {
        auto fb_event = main->message_as_MacroHeadlineEvent();
        auto event = toMacroHeadlineEvent(fb_event);
        std::cout << event << std::endl;
    } else if (main->message_type() == BlpConn::FB::Message_MacroCalendarEvent) {
        auto fb_event = main->message_as_MacroCalendarEvent();
        auto event = toMacroCalendarEvent(fb_event);
        std::cout << event << std::endl;
    } else {
        std::cout << "Unknown message type: " << main->message_type() << std::endl;
    }
    END_PROFILE_FUNCTION()
}

} // namespace BlpConn
