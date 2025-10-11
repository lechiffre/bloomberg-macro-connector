#ifndef _BLPCONN_SERIALIZE_H_
#define _BLPCONN_SERIALIZE_H_

#include <flatbuffers/flatbuffers.h>

#include "blpconn_fb_generated.h"
#include "blpconn_message.h"
#include "blpconn_profiler.h"

namespace BlpConn {

flatbuffers::Offset<FB::HeadlineEconomicEvent>
serializeHeadlineEconomicEvent(flatbuffers::FlatBufferBuilder &builder,
                               const HeadlineEconomicEvent &event);

flatbuffers::Offset<FB::HeadlineCalendarEvent>
serializeHeadlineCalendarEvent(flatbuffers::FlatBufferBuilder &builder,
                               const HeadlineCalendarEvent &event);

flatbuffers::Offset<FB::MacroReferenceData> serializeMacroReferenceData(
    flatbuffers::FlatBufferBuilder& builder,
    const MacroReferenceData& data);

flatbuffers::Offset<FB::MacroHeadlineEvent> serializeMacroHeadlineEvent(
    flatbuffers::FlatBufferBuilder& builder,
    const MacroHeadlineEvent& event);


flatbuffers::Offset<FB::MacroCalendarEvent> serializeMacroCalendarEvent(
    flatbuffers::FlatBufferBuilder& builder,
    const MacroCalendarEvent& event);

flatbuffers::Offset<FB::LogMessage>
serializeLogMessage(flatbuffers::FlatBufferBuilder &builder,
                    const LogMessage &log_message);

// Helper functions
int getLastFileNumber();
std::string fbGetNextFileName(const std::string &dir);
void fbBufferToFile(const uint8_t *buffer, size_t size, const std::string &filename);
;
void fbBuilderToFile(flatbuffers::FlatBufferBuilder &builder,
                     const std::string &filename);

} // namespace BlpConn

#endif // _BLPCONN_SERIALIZE_H_
