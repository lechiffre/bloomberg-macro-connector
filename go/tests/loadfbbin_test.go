package main

import (
	"blpconngo"
	"blpconngo/BlpConn/FB"
	"io"
	"os"
	"path/filepath"
	"testing"

	flatbuffers "github.com/google/flatbuffers/go"
)

// Test deserialization and identification of all files in ../../data
func TestDeserializeAndIdentifyAllFiles(t *testing.T) {
	folder := "../../data"

	macroReferenceDataCount := 0
	macroHeadlineEventCount := 0
	macroCalendarEventCount := 0
	logMessageCount := 0

	err := filepath.Walk(folder, func(path string, info os.FileInfo, err error) error {
		if err != nil {
			return err
		}
		if !info.IsDir() && filepath.Ext(path) == ".bin" {
			t.Run("Processing file: "+filepath.Base(path), func(t *testing.T) {
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
				case FB.MessageMacroReferenceData:
					var fbData = new(FB.MacroReferenceData)
					fbData.Init(unionTable.Bytes, unionTable.Pos)
					event := blpconngo.DeserializeMacroReferenceData(fbData)
					if event.IDBBGlobal == "" {
						t.Fatalf("Empty id_bb_global in MacroReferenceData: %s", path)
					}
					if event.IDBBGlobal != "BBG002SBJ964" {
						t.Errorf("Expected id_bb_global BBG002SBJ964, got %s", event.IDBBGlobal)
					}
					if event.ParsekyableDes != "CATBTOTB Index" {
						t.Errorf("Expected parsekyable_des 'CATBTOTB Index', got %s", event.ParsekyableDes)
					}
					macroReferenceDataCount++

				case FB.MessageMacroHeadlineEvent:
					var fbEvent = new(FB.MacroHeadlineEvent)
					fbEvent.Init(unionTable.Bytes, unionTable.Pos)
					event := blpconngo.DeserializeMacroHeadlineEvent(fbEvent)
					
					if event.EventID != 2167802 {
						t.Errorf("Expected event_id 2167802, got %d", event.EventID)
					}
					if event.ObservationPeriod != "Aug" {
						t.Errorf("Expected observation_period 'Aug', got %s", event.ObservationPeriod)
					}
					
					// Check if it's ACTUAL or REVISION
					if event.EventType == blpconngo.EventTypeActual {
						if event.Value.Value != -6.32 {
							t.Errorf("Expected value -6.32 for ACTUAL event, got %f", event.Value.Value)
						}
					} else if event.EventType == blpconngo.EventTypeRevision {
						if event.Value.Value != -3.82 {
							t.Errorf("Expected value -3.82 for REVISION event, got %f", event.Value.Value)
						}
						if event.PriorEventID != 2167801 {
							t.Errorf("Expected prior_event_id 2167801, got %d", event.PriorEventID)
						}
						if event.PriorObservationPeriod != "Jul" {
							t.Errorf("Expected prior_observation_period 'Jul', got %s", event.PriorObservationPeriod)
						}
					}
					
					macroHeadlineEventCount++

				case FB.MessageMacroCalendarEvent:
					var fbEvent = new(FB.MacroCalendarEvent)
					fbEvent.Init(unionTable.Bytes, unionTable.Pos)
					event := blpconngo.DeserializeMacroCalendarEvent(fbEvent)
					
					if event.EventID != 2167804 && event.EventID != 2167805 && event.EventID != 2167806 {
						t.Errorf("Unexpected event_id: %d", event.EventID)
					}
					if event.EventType != blpconngo.EventTypeCalendar {
						t.Errorf("Expected CALENDAR event type, got %d", event.EventType)
					}
					if event.ReleaseStatus != blpconngo.ReleaseStatusScheduled {
						t.Errorf("Expected SCHEDULED release status, got %d", event.ReleaseStatus)
					}
					
					macroCalendarEventCount++

				case FB.MessageLogMessage:
					var fbEvent = new(FB.LogMessage)
					fbEvent.Init(unionTable.Bytes, unionTable.Pos)
					event := blpconngo.DeserializeLogMessage(fbEvent)
					if event.Message == "" {
						t.Fatalf("Empty message in LogMessage: %s", path)
					}
					logMessageCount++

				default:
					t.Fatalf("Unknown message type %d in file: %s", main.MessageType(), path)
				}
			})
		}
		return nil
	})

	if err != nil {
		t.Fatalf("Error walking through folder: %v", err)
	}

	// Verify we loaded the expected number of each type based on data/README.md
	if logMessageCount != 0 {
		t.Errorf("Expected 0 LogMessage files, got %d", logMessageCount)
	}
	if macroReferenceDataCount != 1 {
		t.Errorf("Expected 1 MacroReferenceData file (fb_000001.bin), got %d", macroReferenceDataCount)
	}
	if macroHeadlineEventCount != 2 {
		t.Errorf("Expected 2 MacroHeadlineEvent files (fb_000005.bin, fb_000006.bin), got %d", macroHeadlineEventCount)
	}
	if macroCalendarEventCount != 3 {
		t.Errorf("Expected 3 MacroCalendarEvent files (fb_000002-000004.bin), got %d", macroCalendarEventCount)
	}
}
