#include <stdint.h>
#include <stddef.h>

extern void NotificationHandler(uint8_t* buffer, size_t len);

void callback(uint8_t* buffer, size_t len) {
    NotificationHandler(buffer, len);
}

