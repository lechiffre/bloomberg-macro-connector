/**
 * Tool to deserialize a flatbuffer generated binary file.
 */

#include <iostream>
#include <fstream>
#include <blpconn_observer.h>

using namespace BlpConn;

int main(int argc, char *argv[]) {
    // Retrieve the finale name containted in the argv[1]
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }
    std::string filename = argv[1];
    // Open the file
    std::ifstream file(filename, std::ios::binary);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return 1;
    }
    // Read the file into a string
    std::string content((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    const uint8_t *buffer = reinterpret_cast<const uint8_t *>(content.data());
    const size_t size = content.size();
    defaultObserver(buffer, size);
}

