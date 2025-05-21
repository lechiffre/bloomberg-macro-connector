#ifndef _BLPCONN_OBSERVER_H
#define _BLPCONN_OBSERVER_H

#include <cstddef>
#include <cstdint>

namespace BlpConn {

/**
 * Definition for observer functions. A client program can provide one or more
 * observer functions to handle serialized values. These functions will receive
 * the type of object being received, a pointer to the serialized buffer, and
 * its size.  The client program is responsible for parsing and processing the
 * data. Data is serialized using FlatBuffers.
 *
 * @param type The type of the object being received.
 * @param buffer A pointer to the serialized buffer.
 * @param size The size of the serialized buffer.
 */
typedef void (*ObserverFunc)(const uint8_t *buffer, size_t size);

void defaultObserver(const uint8_t *buffer, size_t size);
}  // namespace BlpConn

#endif  // _BLPCONN_OBSERVER_H
