#pragma once

#include <cstdint>
namespace BlpConn {
namespace Message {

struct DateTime {
    uint64_t microseconds;
    int16_t offset;
};

} // namespace Message
} // namespace BlpConn
