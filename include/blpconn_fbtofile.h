#ifndef BLPCONN_FBTOFILE_H
#define BLPCONN_FBTOFILE_H

#include <string>
#include <flatbuffers/flatbuffers.h>

namespace BlpConn {

/**
 * Get the next available filename in the format fb_XXXXXX.bin
 * where XXXXXX is a 6-digit number with leading zeros.
 * 
 * @param dir The directory to check and create the file in
 * @return The full path to the next file
 */
std::string fbGetNextFileName(const std::string& dir);

/**
 * Write a FlatBuffer buffer to a binary file.
 * 
 * @param buffer Pointer to the buffer data
 * @param size Size of the buffer in bytes
 * @param filename Path to the file to write
 */
void fbBufferToFile(const uint8_t* buffer, size_t size, const std::string& filename);

/**
 * Write a FlatBufferBuilder's buffer to a binary file.
 * 
 * @param builder The FlatBufferBuilder containing the data
 * @param filename Path to the file to write
 */
void fbBuilderToFile(flatbuffers::FlatBufferBuilder& builder, const std::string& filename);

} // namespace BlpConn

#endif // BLPCONN_FBTOFILE_H
