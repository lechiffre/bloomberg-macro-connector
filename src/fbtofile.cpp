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
int getLastFileNumber() {
    // Open the directory and read the files.
    std::string filename = "fb_000000.bin";
    int n = 0;
    // Open the directory and read the files.
    DIR* dir = opendir(".");
    struct dirent* ent;
    while ((ent = readdir(dir)) != NULL) {
        std::string name(ent->d_name);
        if (name.find("fb_") == 0 && name.find(".bin") != std::string::npos) {
            // Extract the number from the file name.
            std::string number = name.substr(3, 6);
            int num = std::stoi(number);
            if (num > n) {
                n = num;
            }
        }
    }
    closedir(dir);
    return n;
}

/**
 * Check the last file saved. Extracts the number from the file name.
 * Increases the number by one and returns the new file name.
 * The number in the file is leading by 6 zeros.
 */
std::string fbGetNextFileName(const std::string& dir) {
    if (chdir(dir.c_str()) == -1) {
        std::cerr << "Error changing directory to: " << dir << std::endl;
        throw std::runtime_error("Error changing directory to debug directory");
    }
    int n = getLastFileNumber();
    n++;
    // The new file names is fb_XXXXXXX.bin,
    // where XXXXXX is the number with leading zeros.
    std::string filename = "fb_";
    std::string number = std::to_string(n);
    while (number.length() < 6) {
        number = "0" + number;
    }
    filename += number;
    filename += ".bin";
    // Change the current directory back to the original directory.
    if (chdir("..") == -1) {
        std::cerr << "Error changing directory back to original" << std::endl;
        throw std::runtime_error("Error changing directory back to original");
    }
    // Create a full path to the file.
    // Check if the last character of the directory is a slash.
    if (dir[dir.length() - 1] != '/') {
        filename = std::string(dir) + "/" + filename;
    } else {
        // If the last character is a slash, just append the filename.
        filename = std::string(dir) + filename;
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
