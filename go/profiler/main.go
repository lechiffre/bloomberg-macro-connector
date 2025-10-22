package main

/*
#cgo CFLAGS: -g -DENABLE_PROFILING
#cgo LDFLAGS: -L../../lib -lblpapi3_64 -lblpconngo -lblpconn -lstdc++ 

#include <stdint.h>
#include <stddef.h>

extern void MainHandler(uint8_t* buffer, size_t len);

static void custom_callback(uint8_t* buffer, size_t len) {
    MainHandler(buffer, len);
}

// Go requires a pointer to the callback function. This auxiliar
// function returns such pointer.
static void* get_custom_callback() {
    return (void*)custom_callback;
}
*/
import "C"

import (
	"fmt"
	"unsafe"
	"blpconngo"
	"blpconngo/BlpConn/FB"
	"time"
	"log"
	flatbuffers "github.com/google/flatbuffers/go"
)

//export MainHandler
func MainHandler(buffer *C.uchar, len C.size_t) {
	if buffer == nil || len == 0 {
		fmt.Println("Invalid buffer or length")
		return
	}
	bufferSlice := C.GoBytes(unsafe.Pointer(buffer), C.int(len))
	NativeHandler(bufferSlice)
}

// Global state
var ctxManaged = blpconngo.NewManagedContext()
var ctxSimple = blpconngo.NewContext()
var referenceMap = blpconngo.NewReferenceMap()

func NativeHandler(bufferSlice []byte) {
	main := FB.GetRootAsMain(bufferSlice, 0)
	if main == nil {
		fmt.Println("Failed to parse FlatBuffers main object")
		return
	}
	unionTable := new(flatbuffers.Table)
	if main.Message(unionTable) {
		switch main.MessageType() {
			case FB.MessageLogMessage:
				var fbEvent = new(FB.LogMessage)
				fbEvent.Init(unionTable.Bytes, unionTable.Pos)
				event := blpconngo.DeserializeLogMessage(fbEvent)
				referenceMap.LookAndRemove(event)
				fmt.Println("Log Message")
				fmt.Println(event)
			case FB.MessageMacroReferenceData:
				var fbEvent = new(FB.MacroReferenceData)
				fbEvent.Init(unionTable.Bytes, unionTable.Pos)
				event := blpconngo.DeserializeMacroReferenceData(fbEvent)
				referenceMap.Add(event)
				fmt.Println("Macro Reference Data:")
				fmt.Println(event)
			case FB.MessageMacroHeadlineEvent:
				var fbEvent = new(FB.MacroHeadlineEvent)
				fbEvent.Init(unionTable.Bytes, unionTable.Pos)
				_event := blpconngo.DeserializeMacroHeadlineEvent(fbEvent)
				event := referenceMap.FillHeadlineEvent(_event)
				fmt.Println("Macro Headline Event:")
				fmt.Println(event)
			case FB.MessageMacroCalendarEvent:
				var fbEvent = new(FB.MacroCalendarEvent)
				fbEvent.Init(unionTable.Bytes, unionTable.Pos)
				_event := blpconngo.DeserializeMacroCalendarEvent(fbEvent)
				event := referenceMap.FillCalendarEvent(_event)
				fmt.Println("Macro Calendar Event:")
				fmt.Println(event)
			default:
				fmt.Println("Unknown message type")
		}
	}
}

func main() {
	configPath := "./config.json"
	callback := (*byte)(C.get_custom_callback())
	ctxManaged.AddNotificationHandler(callback)
	if ok := ctxManaged.InitializeSession(configPath); !ok {
		log.Fatal("Failed to initialize session")
	}
	defer ctxManaged.ManagedShutdown()
	ctxManaged.SubscribeTicker("INJCJC Index")
	time.Sleep(10 * time.Second)
}
