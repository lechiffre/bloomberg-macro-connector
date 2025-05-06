#include "local.h"

extern void NotificationHandler(uint8_t *, size_t);
void Callback(uint8_t* buffer, size_t len) {
    NotificationHandler(buffer, len);
}