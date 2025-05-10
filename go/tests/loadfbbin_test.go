package tests

import (
	"blpconngo"
	"blpconngo/BlpConn/FB"
	"io"
	"os"
	"path/filepath"
	"testing"

	flatbuffers "github.com/google/flatbuffers/go"
)

// Test deserialization and identification of all files in tests/fbbin
func TestDeserializeAndIdentifyAllFiles(t *testing.T) {
	folder := "./fbbin"

	err := filepath.Walk(folder, func(path string, info os.FileInfo, err error) error {
		if err != nil {
			return err
		}
		if !info.IsDir() {
			t.Run("Processing file: "+path, func(t *testing.T) {
				file, err := os.Open(path)
				if err != nil {
					t.Fatalf("Error opening file: %v", err)
				}
				defer file.Close()

				data, err := io.ReadAll(file)
				if err != nil {
					t.Fatalf("Error reading file: %v", err)
				}

				main := FB.GetRootAsMain(data, 0)
				if main == nil {
					t.Fatalf("Failed to parse FlatBuffers main object in file: %s", path)
				}

				unionTable := new(flatbuffers.Table)
				if !main.Message(unionTable) {
					t.Fatalf("Failed to retrieve message table in file: %s", path)
				}

				switch main.MessageType() {
				case FB.MessageHeadlineEconomicEvent:
					var fbEvent = new(FB.HeadlineEconomicEvent)
					fbEvent.Init(unionTable.Bytes, unionTable.Pos)
					event := blpconngo.DeserializeHeadlineEconomicEvent(fbEvent)
					if event.IDBBGlobal == "" {
						t.Fatalf("Empty id_bb_global in HeadlineEconomicEvent: %s", path)
					}
				case FB.MessageHeadlineCalendarEvent:
					var fbEvent = new(FB.HeadlineCalendarEvent)
					fbEvent.Init(unionTable.Bytes, unionTable.Pos)
					event := blpconngo.DeserializeHeadlineCalendarEvent(fbEvent)
					if event.IDBBGlobal == "" {
						t.Fatalf("Empty id_bb_global in HeadlineCalendarEvent: %s", path)
					}
				case FB.MessageLogMessage:
					var fbEvent = new(FB.LogMessage)
					fbEvent.Init(unionTable.Bytes, unionTable.Pos)
					event := blpconngo.DeserializeLogMessage(fbEvent)
					if event.Message == "" {
						t.Fatalf("Empty message in LogMessage: %s", path)
					}
				default:
					t.Fatalf("Unknown message type in file: %s", path)
				}
			})
		}
		return nil
	})

	if err != nil {
		t.Fatalf("Error walking through folder: %v", err)
	}
}
