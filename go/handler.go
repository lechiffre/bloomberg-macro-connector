package blpconngo

import (
	"encoding/json"
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
			case FB.MessageMacroReferenceData:
				var fbEvent = new(FB.MacroReferenceData)
				fbEvent.Init(unionTable.Bytes, unionTable.Pos)
				event := DeserializeMacroReferenceData(fbEvent)
				fmt.Println("MacroReferenceData")
				fmt.Println(event)
				data, err := json.Marshal(event); if err != nil {
					fmt.Println("Error parsing MacroReferenceData")
					break;
				}
				fmt.Println(string(data))
			case FB.MessageMacroHeadlineEvent:
				var fbEvent = new(FB.MacroHeadlineEvent)
				fbEvent.Init(unionTable.Bytes, unionTable.Pos)
				event := DeserializeMacroHeadlineEvent(fbEvent)
				fmt.Println("MacroHeadlineEvent")
				fmt.Println(event)
				data, err := json.Marshal(event); if err != nil {
					fmt.Println("Error parsing MacroHeadlineEvent")
					break;
				}
				fmt.Println(string(data))
			case FB.MessageMacroCalendarEvent:
				var fbEvent = new(FB.MacroCalendarEvent)
				fbEvent.Init(unionTable.Bytes, unionTable.Pos)
				event := DeserializeMacroCalendarEvent(fbEvent)
				fmt.Println("MacroCalendarEvent")
				fmt.Println(event)
				data, err := json.Marshal(event); if err != nil {
					fmt.Println("Error parsing MacroCalendarEvent")
					break;
				}
				fmt.Println(string(data))
			default:
				fmt.Println("Unknown message type")
		}
	}
}
