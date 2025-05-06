package main

/*
#include "local.h"
#include <stddef.h>
#cgo LDFLAGS: -L../lib -lblpapi3_64 -lblpconngo -lblpconn -lstdc++
#include <stdlib.h>
*/
import "C"
import (
	"fmt"
	"gocli/BlpConn/FB"
	"unsafe"
)

var Callback = (*byte)(unsafe.Pointer(C.Callback))

//export NotificationHandler
func NotificationHandler(buffer *C.uchar, len C.size_t) {
	if buffer == nil || len == 0 {
		fmt.Println("Invalid buffer or length")
		return
	}
	bufferSlice := C.GoBytes(unsafe.Pointer(buffer), C.int(len))
	main := FB.GetRootAsMain(bufferSlice, 0)
	if main == nil {
		fmt.Println("Failed to parse FlatBuffers main object")
		return
	}
	switch main.MessageType() {
	case FB.MessageHeadlineEconomicEvent:
		var fbEvent = FB.GetRootAsHeadlineEconomicEvent(bufferSlice, 0)
		event := DeserializeHeadlineEconomicEvent(fbEvent)
		fmt.Println(event)
	case FB.MessageHeadlineCalendarEvent:
		var fbEvent = FB.GetRootAsHeadlineCalendarEvent(bufferSlice, 0)
		event := DeserializeHeadlineCalendarEvent(fbEvent)
		fmt.Println(event)
	case FB.MessageLogMessage:
		var fbLogMessage = FB.GetRootAsLogMessage(bufferSlice, 0)
		logMessage := DeserializeLogMessage(fbLogMessage)
		fmt.Println(logMessage)
	default:
		fmt.Println("Unknown message type")
	}
}
