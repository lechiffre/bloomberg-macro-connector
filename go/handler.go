package blpconngo

import (
	"blpconngo/BlpConn/FB"
	flatbuffers "github.com/google/flatbuffers/go"
	"fmt"
)

func NativeHandler(bufferSlice []byte) {
	main := FB.GetRootAsMain(bufferSlice, 0)
	if main == nil {
		fmt.Println("Failed to parse FlatBuffers main object")
		return
	}
	unionTable := new(flatbuffers.Table)
	if main.Message(unionTable) {
		switch main.MessageType() {
			case FB.MessageHeadlineEconomicEvent:
				var fbEvent = new(FB.HeadlineEconomicEvent)
				fbEvent.Init(unionTable.Bytes, unionTable.Pos)
				event := DeserializeHeadlineEconomicEvent(fbEvent)
				fmt.Println(event)
			case FB.MessageHeadlineCalendarEvent:
				var fbEvent = new(FB.HeadlineCalendarEvent)
				fbEvent.Init(unionTable.Bytes, unionTable.Pos)
				event := DeserializeHeadlineCalendarEvent(fbEvent)
				fmt.Println(event)
			case FB.MessageLogMessage:
				var fbEvent = new(FB.LogMessage)
				fbEvent.Init(unionTable.Bytes, unionTable.Pos)
				event := DeserializeLogMessage(fbEvent)
				fmt.Println(event)
			default:
				fmt.Println("Unknown message type")
		}
	}
}
