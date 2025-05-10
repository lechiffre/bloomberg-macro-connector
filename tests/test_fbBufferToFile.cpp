#include <gtest/gtest.h>
#include <fstream>
#include <string>
#include "blpconn_serialize.h"

using namespace BlpConn;

// Test for getLastFileNumber
TEST(FbToFileTest, GetLastFileNumber) {
    // Setup: Create mock files
    system("touch fb_000001.bin fb_000002.bin");
    EXPECT_EQ(getLastFileNumber(), 2);
    system("rm fb_000001.bin fb_000002.bin");
}

// Test for fbGetNextFileName
TEST(FbToFileTest, GetNextFileName) {
    // Setup: Create a mock file
    system("touch fb_000000.bin");
    EXPECT_EQ(fbGetNextFileName("."), "./fb_000001.bin");
    system("rm fb_000000.bin");
}

// Test for fbBufferToFile
TEST(FbToFileTest, BufferToFile) {
    uint8_t testBuffer[] = {1, 2, 3, 4, 5};
    const std::string filename = fbGetNextFileName(".");
    fbBufferToFile(testBuffer, sizeof(testBuffer), filename);
    std::ifstream file(filename, std::ios::binary);
    EXPECT_TRUE(file.good());
    // Compare the contents of the file with the buffer
    file.seekg(0, std::ios::end);
    size_t fileSize = file.tellg();
    file.seekg(0, std::ios::beg);
    std::vector<uint8_t> fileBuffer(fileSize);
    file.read(reinterpret_cast<char*>(fileBuffer.data()), fileSize);
    EXPECT_EQ(fileSize, sizeof(testBuffer));
    EXPECT_EQ(memcmp(fileBuffer.data(), testBuffer, sizeof(testBuffer)), 0);
    // Clean up
    file.close();
    system(("rm " + filename).c_str());
}

// Test for fbBufferToFile with invalid buffer
TEST(FbToFileTest, BufferToFile_InvalidSize) {
    uint8_t* invalidBuffer = nullptr;
    const std::string filename = fbGetNextFileName(".");
    EXPECT_THROW(fbBufferToFile(invalidBuffer, 5, filename), std::runtime_error);
    // Clean up
    system(("rm " + filename).c_str());
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

