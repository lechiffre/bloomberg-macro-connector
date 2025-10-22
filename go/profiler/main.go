package main

/*
#cgo CFLAGS: -g -DENABLE_PROFILING
#cgo LDFLAGS: -L../../lib -lblpapi3_64 -lblpconngo -lblpconn -lstdc++ 

#include <stdint.h>
#include <stddef.h>

extern void MainHandler(uint8_t* buffer, size_t len);

// The C wrapper over the Go callback function.
static void custom_callback(uint8_t* buffer, size_t len) {
    MainHandler(buffer, len);
}

// Go requires a pointer to the C callback function. Because the callback
// function is in the same file, the linker will no be able to find it.
// It is because of that we need an auxiliar function to return such pointer.
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
var callback = (*byte)(C.get_custom_callback())
var startManaged time.Time
var startSimple time.Time
var managedCorrID uint64 = 1  // It is only one subscription

func timeTrack(start time.Time, name string) {
	elapsed := time.Since(start)
	fmt.Printf(">%s,%s\n", name, elapsed)
}

func reviewLogMessage(event blpconngo.LogMessageType) {
	if event.Module == blpconngo.ModuleSubscription {
		if blpconngo.SubscriptionStatus(event.Status) ==  blpconngo.SubscriptionSuccess {
			if event.CorrelationID == managedCorrID {
				timeTrack(startManaged, "managed")
			} else if event.CorrelationID == 10 {
				timeTrack(startSimple, "simple")
			}
		}
	}
}

// A custom handler dedicated to detect log messages
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
				reviewLogMessage(event)
			default:
				fmt.Println("Unknown message type")
		}
	}
}

func ManagedProfile() {
	ctx := blpconngo.NewManagedContext()
	configPath := "./config.json"
	ctx.AddNotificationHandler(callback)
	if ok := ctx.InitializeSession(configPath); !ok {
		log.Fatal("Failed to initialize session")
	}
	defer ctx.ManagedShutdown()
	startManaged = time.Now()
	ctx.SubscribeTicker("INJCJC Index")
	time.Sleep(1 * time.Second)
}

func SimpleProfile() {
	ctx := blpconngo.NewContext()
	ctx.AddNotificationHandler(callback)
	configPath := "./config.json"
	res := ctx.InitializeSession(configPath)
	if !res {
		log.Fatal("Failed to initialize session")
	}
	defer ctx.ShutdownSession()
	startSimple = time.Now()
	request := blpconngo.NewSubscriptionRequest()
	request.SetTopic("INJCJC Index")
	request.SetCorrelation_id(10)
	ctx.Subscribe(request)
	time.Sleep(1 * time.Second)
	ctx.Unsubscribe(request)
}

func main() {
	ManagedProfile()
	SimpleProfile()
}
