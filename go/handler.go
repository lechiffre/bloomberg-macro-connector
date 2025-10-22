package blpconngo

import (
	_ "encoding/json"
	"blpconngo/BlpConn/FB"
	flatbuffers "github.com/google/flatbuffers/go"
	"fmt"
)

// A reference map is created to handle the extension of the
// economic and calendar event data.
var referenceMap = NewReferenceMap()

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
				event := DeserializeLogMessage(fbEvent)
				referenceMap.LookAndRemove(event)
				fmt.Println("Log Message")
				fmt.Println(event)
			case FB.MessageMacroReferenceData:
				var fbEvent = new(FB.MacroReferenceData)
				fbEvent.Init(unionTable.Bytes, unionTable.Pos)
				event := DeserializeMacroReferenceData(fbEvent)
				referenceMap.Add(event)
				fmt.Println("Macro Reference Data:")
				fmt.Println(event)
			case FB.MessageMacroHeadlineEvent:
				var fbEvent = new(FB.MacroHeadlineEvent)
				fbEvent.Init(unionTable.Bytes, unionTable.Pos)
				_event := DeserializeMacroHeadlineEvent(fbEvent)
				event := referenceMap.FillHeadlineEvent(_event)
				fmt.Println("Macro Headline Event:")
				fmt.Println(event)
			case FB.MessageMacroCalendarEvent:
				var fbEvent = new(FB.MacroCalendarEvent)
				fbEvent.Init(unionTable.Bytes, unionTable.Pos)
				_event := DeserializeMacroCalendarEvent(fbEvent)
				event := referenceMap.FillCalendarEvent(_event)
				fmt.Println("Macro Calendar Event:")
				fmt.Println(event)
			default:
				fmt.Println("Unknown message type")
		}
	}
}
