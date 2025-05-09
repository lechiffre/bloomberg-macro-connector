/**
 * In order to pass the callback function from Go to C++.
 *
 * 1. The Go notification function must be encapsulated
 *    inside a C function.
 * 2. A Go pointer to that C function should be created
 * 3. That pointer is the one that is passed to the C++ code.
 */

#ifndef _CALLBACK_H
#define _CALLBACK_H

#include <stdint.h>

void callback(uint8_t* buffer, size_t len);

#endif // _CALLBACK_H
