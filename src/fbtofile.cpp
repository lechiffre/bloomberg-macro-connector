/**
  * This module contains functions to convert FlatBuffers to file,
  * in order to compile examples for testing.
  */

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <flatbuffers/flatbuffers.h>
#include "blpconn_profiler.h"

namespace BlpConn {

/**
 * Get the last file number from the file name.
 * The file name is fb_XXXXXXX.bin, where XXXXXXX is the number.
 * The function returns the number.
 */
static int getLastFileNumber(const std::string& dir) {
    int n = 0;
    DIR* dirp = opendir(dir.c_str());
    if (!dirp) {
        return n;
    }
    struct dirent* ent;
    while ((ent = readdir(dirp)) != NULL) {
        std::string name(ent->d_name);
        if (name.find("fb_") == 0 && name.find(".bin") != std::string::npos) {
            // Extract the number from the file name.
            std::string number = name.substr(3, 6);
            try {
                int num = std::stoi(number);
                if (num > n) {
                    n = num;
                }
            } catch (...) {
                // Ignore invalid filenames
            }
        }
    }
    closedir(dirp);
    return n;
}

/**
 * Check the last file saved. Extracts the number from the file name.
 * Increases the number by one and returns the new file name.
 * The number in the file is leading by 6 zeros.
 */
std::string fbGetNextFileName(const std::string& dir) {
    int n = getLastFileNumber(dir);
    n++;
    // The new file name is fb_XXXXXXX.bin,
    // where XXXXXX is the number with leading zeros.
    std::string number = std::to_string(n);
    while (number.length() < 6) {
        number = "0" + number;
    }
    std::string filename = "fb_" + number + ".bin";
    
    // Create a full path to the file.
    // Check if the last character of the directory is a slash.
    if (dir[dir.length() - 1] != '/') {
        filename = dir + "/" + filename;
    } else {
        filename = dir + filename;
    }
    return filename;
}

/**
 * Writes a flatbuffer to disk.
 *
 * @param buffer The buffer to write.
 * @param size The size of the buffer.
 * @param filename The name of the file to write to.
 */
void fbBufferToFile(const uint8_t* buffer, size_t size, const std::string& filename) {
    if (!buffer) {
        std::cerr << "Buffer is null" << std::endl;
        throw std::runtime_error("Buffer is null");
    }
    std::ofstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file for writing: " << filename << std::endl;
        throw std::runtime_error("Error opening file for writing");
    }
    // Write the buffer to the file.
    file.write(reinterpret_cast<const char*>(buffer), size);
    // Close the file
    file.close();
    std::cout << "Buffer saved to: " << filename << std::endl;
}

void fbBuilderToFile(flatbuffers::FlatBufferBuilder& builder, const std::string& filename) {
    PROFILE_FUNCTION()
    // Get the buffer pointer and size.
    uint8_t* buffer = builder.GetBufferPointer();
    size_t size = builder.GetSize();
    // Write the buffer to the file.
    fbBufferToFile(buffer, size, filename);
    END_PROFILE_FUNCTION()
}

} // namespace BlpConn
