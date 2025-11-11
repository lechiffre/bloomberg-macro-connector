#include <gtest/gtest.h>
#include <fstream>
#include <vector>
#if __cplusplus >= 201703L && __has_include(<filesystem>)
    #include <filesystem>
    namespace fs = std::filesystem;
#elif __has_include(<experimental/filesystem>)
    #include <experimental/filesystem>
    namespace fs = std::experimental::filesystem;
#else
    #error "No filesystem support available. Please use a compiler with C++17 or later."
#endif
#include <flatbuffers/flatbuffers.h>
#include <blpconn_fb_generated.h>
#include <blpconn_deserialize.h>

using namespace BlpConn;

// Helper function to read binary file into a vector
std::vector<uint8_t> readBinaryFile(const std::string& filepath) {
    std::ifstream file(filepath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filepath);
    }
    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<uint8_t> buffer(size);
    if (!file.read(reinterpret_cast<char*>(buffer.data()), size)) {
        throw std::runtime_error("Failed to read file: " + filepath);
    }
    return buffer;
}

// Test deserialization and identification of all files in data folder
TEST(FBBinDeserializeTest, DeserializeAndIdentifyAllFiles) {
    const std::string folder = "data";
    
    int log_message_count = 0;
    int macro_reference_data_count = 0;
    int macro_headline_event_count = 0;
    int macro_calendar_event_count = 0;

    for (const auto& entry : fs::directory_iterator(folder)) {
        if (fs::is_regular_file(entry) && entry.path().extension() == ".bin") {
            const std::string filepath = entry.path().string();
            SCOPED_TRACE("Processing file: " + filepath);

            try {
                auto buffer = readBinaryFile(filepath);

                flatbuffers::Verifier verifier(buffer.data(), buffer.size());
                if (!BlpConn::FB::VerifyMessageVector(verifier, nullptr, nullptr)) {
                    ADD_FAILURE() << "Failed to verify FlatBuffer file: " << filepath;
                    continue;
                }

                auto main = flatbuffers::GetRoot<BlpConn::FB::Main>(buffer.data());
                if (!main) {
                    ADD_FAILURE() << "Failed to parse FlatBuffer root in file: " << filepath;
                    continue;
                }

                switch (main->message_type()) {
                    case FB::Message_MacroReferenceData: {
                        auto fb_data = main->message_as_MacroReferenceData();
                        if (!fb_data) {
                            ADD_FAILURE() << "Failed to parse MacroReferenceData in file: " << filepath;
                            continue;
                        }
                        MacroReferenceData data = toMacroReferenceData(fb_data);
                        EXPECT_FALSE(data.id_bb_global.empty());
                        macro_reference_data_count++;
                        break;
                    }
                    case FB::Message_MacroHeadlineEvent: {
                        auto fb_event = main->message_as_MacroHeadlineEvent();
                        if (!fb_event) {
                            ADD_FAILURE() << "Failed to parse MacroHeadlineEvent in file: " << filepath;
                            continue;
                        }
                        MacroHeadlineEvent event = toMacroHeadlineEvent(fb_event);
                        EXPECT_NE(event.event_id, 0);
                        macro_headline_event_count++;
                        break;
                    }
                    case FB::Message_MacroCalendarEvent: {
                        auto fb_event = main->message_as_MacroCalendarEvent();
                        if (!fb_event) {
                            ADD_FAILURE() << "Failed to parse MacroCalendarEvent in file: " << filepath;
                            continue;
                        }
                        MacroCalendarEvent event = toMacroCalendarEvent(fb_event);
                        EXPECT_NE(event.event_id, 0);
                        macro_calendar_event_count++;
                        break;
                    }
                    case FB::Message_LogMessage: {
                        auto fb_message = main->message_as_LogMessage();
                        if (!fb_message) {
                            ADD_FAILURE() << "Failed to parse LogMessage in file: " + filepath;
                            continue;
                        }
                        LogMessage log_message = toLogMessage(fb_message);
                        EXPECT_FALSE(log_message.message.empty());
                        log_message_count++;
                        break;
                    }
                    default:
                        ADD_FAILURE() << "Unknown message type in file: " + filepath;
                }
            } catch (const std::exception& e) {
                ADD_FAILURE() << "Exception while processing file " + filepath + ": " << e.what();
            } catch (...) {
                ADD_FAILURE() << "Unknown error while processing file " + filepath;
            }
        }
    }
    
    // Verify we loaded the expected number of each type based on data/README.md
    EXPECT_EQ(log_message_count, 11) << "Expected 11 LogMessage files";
    EXPECT_EQ(macro_reference_data_count, 1) << "Expected 1 MacroReferenceData file";
    EXPECT_EQ(macro_headline_event_count, 2) << "Expected 2 MacroHeadlineEvent files";
    EXPECT_EQ(macro_calendar_event_count, 4) << "Expected 4 MacroCalendarEvent files";
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
