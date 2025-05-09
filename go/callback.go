package blpconngo

/*
#include <callback.h>
#cgo LDFLAGS: -L../lib -lblpapi3_64 -lblpconngo -lblpconn -lstdc++
*/
import "C"
import (
	"fmt"
	"unsafe"
)

var Callback = (*byte)(unsafe.Pointer(C.callback))

//export NotificationHandler
func NotificationHandler(buffer *C.uchar, len C.size_t) {
	if buffer == nil || len == 0 {
		fmt.Println("Invalid buffer or length")
		return
	}
	bufferSlice := C.GoBytes(unsafe.Pointer(buffer), C.int(len))
	NativeHandler(bufferSlice)
}
