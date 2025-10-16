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

// Test deserialization and identification of all files in tests/fbbin
TEST(FBBinDeserializeTest, DeserializeAndIdentifyAllFiles) {
    const std::string folder = "tests/fbbin";

    for (const auto& entry : fs::directory_iterator(folder)) {
        if (fs::is_regular_file(entry)) {
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
                    case FB::Message_HeadlineEconomicEvent: {
                        auto fb_event = main->message_as_HeadlineEconomicEvent();
                        if (!fb_event) {
                            ADD_FAILURE() << "Failed to parse HeadlineEconomicEvent in file: " << filepath;
                            continue;
                        }
                        HeadlineEconomicEvent event = toHeadlineEconomicEvent(fb_event);
                        EXPECT_FALSE(event.id_bb_global.empty());
                        break;
                    }
                    case FB::Message_HeadlineCalendarEvent: {
                        auto fb_event = main->message_as_HeadlineCalendarEvent();
                        if (!fb_event) {
                            ADD_FAILURE() << "Failed to parse HeadlineCalendarEvent in file: " + filepath;
                            continue;
                        }
                        HeadlineCalendarEvent event = toHeadlineCalendarEvent(fb_event);
                        EXPECT_FALSE(event.id_bb_global.empty());
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
}

int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
