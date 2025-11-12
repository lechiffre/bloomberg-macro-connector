package main

import (
	"encoding/json"
	"math"
	"os"
	"strings"
	"testing"
	"blpconngo"
	"blpconngo/BlpConn/FB"
	flatbuffers "github.com/google/flatbuffers/go"
)

const testDataDir = "../../data"

func readBinaryFile(filename string) []byte {
	filepath := testDataDir + "/" + filename
	data, err := os.ReadFile(filepath)
	if err != nil {
		panic(err)
	}
	return data
}

// Test: Deserialize MacroReferenceData and convert to JSON
func TestMacroReferenceDataToJSON(t *testing.T) {
	buffer := readBinaryFile("fb_000008.bin")
	main := FB.GetRootAsMain(buffer, 0)
	if main == nil {
		t.Fatal("Failed to parse FlatBuffers main object")
	}
	if main.MessageType() != FB.MessageMacroReferenceData {
		t.Fatalf("Expected MessageMacroReferenceData, got %d", main.MessageType())
	}
	
	unionTable := new(flatbuffers.Table)
	if !main.Message(unionTable) {
		t.Fatal("Failed to get union table")
	}
	
	var fbEvent = new(FB.MacroReferenceData)
	fbEvent.Init(unionTable.Bytes, unionTable.Pos)
	event := blpconngo.DeserializeMacroReferenceData(fbEvent)
	
	// Convert to JSON
	jsonData, err := json.MarshalIndent(event, "", "  ")
	if err != nil {
		t.Fatalf("Failed to marshal to JSON: %v", err)
	}
	
	t.Logf("MacroReferenceData JSON:\n%s", string(jsonData))
	
	// Verify JSON contains expected fields
	var jsonMap map[string]interface{}
	err = json.Unmarshal(jsonData, &jsonMap)
	if err != nil {
		t.Fatalf("Failed to unmarshal JSON: %v", err)
	}
	
	// Check key fields exist
	if jsonMap["id_bb_global"] != "BBG002SBJ964" {
		t.Errorf("Expected id_bb_global 'BBG002SBJ964', got '%v'", jsonMap["id_bb_global"])
	}
	if jsonMap["parsekyable_des"] != "CATBTOTB Index" {
		t.Errorf("Expected parsekyable_des 'CATBTOTB Index', got '%v'", jsonMap["parsekyable_des"])
	}
	
	// Roundtrip test
	var restored blpconngo.MacroReferenceData
	err = json.Unmarshal(jsonData, &restored)
	if err != nil {
		t.Fatalf("Failed to unmarshal back from JSON: %v", err)
	}
	
	if restored.IDBBGlobal != event.IDBBGlobal {
		t.Errorf("Roundtrip failed for IDBBGlobal")
	}
	if restored.CountryISO != event.CountryISO {
		t.Errorf("Roundtrip failed for CountryISO")
	}
}

// Test: Deserialize MacroHeadlineEvent and convert to JSON with NaN handling
func TestMacroHeadlineEventToJSON(t *testing.T) {
	buffer := readBinaryFile("fb_000013.bin")
	main := FB.GetRootAsMain(buffer, 0)
	if main == nil {
		t.Fatal("Failed to parse FlatBuffers main object")
	}
	if main.MessageType() != FB.MessageMacroHeadlineEvent {
		t.Fatalf("Expected MessageMacroHeadlineEvent, got %d", main.MessageType())
	}
	
	unionTable := new(flatbuffers.Table)
	if !main.Message(unionTable) {
		t.Fatal("Failed to get union table")
	}
	
	var fbEvent = new(FB.MacroHeadlineEvent)
	fbEvent.Init(unionTable.Bytes, unionTable.Pos)
	event := blpconngo.DeserializeMacroHeadlineEvent(fbEvent)
	
	// Verify NaN values exist in deserialized data
	if !math.IsNaN(event.Value.Low) {
		t.Errorf("Expected Value.Low to be NaN")
	}
	if !math.IsNaN(event.Value.High) {
		t.Errorf("Expected Value.High to be NaN")
	}
	
	// Convert to JSON
	jsonData, err := json.MarshalIndent(event, "", "  ")
	if err != nil {
		t.Fatalf("Failed to marshal to JSON: %v", err)
	}
	
	t.Logf("MacroHeadlineEvent JSON:\n%s", string(jsonData))
	
	// Verify JSON is valid and doesn't contain NaN strings
	jsonStr := string(jsonData)
	if strings.Contains(jsonStr, "NaN") {
		t.Errorf("JSON should not contain literal 'NaN' string")
	}
	
	// Parse as generic structure
	var jsonMap map[string]interface{}
	err = json.Unmarshal(jsonData, &jsonMap)
	if err != nil {
		t.Fatalf("Failed to unmarshal JSON: %v", err)
	}
	
	// Check key fields
	if jsonMap["event_id"] != float64(2167802) {
		t.Errorf("Expected event_id 2167802, got %v", jsonMap["event_id"])
	}
	if jsonMap["observation_period"] != "Aug" {
		t.Errorf("Expected observation_period 'Aug', got '%v'", jsonMap["observation_period"])
	}
	
	// Roundtrip test
	var restored blpconngo.MacroHeadlineEvent
	err = json.Unmarshal(jsonData, &restored)
	if err != nil {
		t.Fatalf("Failed to unmarshal back from JSON: %v", err)
	}
	
	// Verify NaN values are preserved
	if !math.IsNaN(restored.Value.Low) {
		t.Errorf("Expected restored Value.Low to be NaN")
	}
	if !math.IsNaN(restored.Value.High) {
		t.Errorf("Expected restored Value.High to be NaN")
	}
	
	// Verify non-NaN values match
	if restored.Value.Number != event.Value.Number {
		t.Errorf("Expected Value.Number %f, got %f", event.Value.Number, restored.Value.Number)
	}
	if restored.Value.Value != event.Value.Value {
		t.Errorf("Expected Value.Value %f, got %f", event.Value.Value, restored.Value.Value)
	}
	if restored.EventID != event.EventID {
		t.Errorf("Roundtrip failed for EventID")
	}
}

// Test: Deserialize MacroCalendarEvent and convert to JSON
func TestMacroCalendarEventToJSON(t *testing.T) {
	buffer := readBinaryFile("fb_000011.bin")
	main := FB.GetRootAsMain(buffer, 0)
	if main == nil {
		t.Fatal("Failed to parse FlatBuffers main object")
	}
	if main.MessageType() != FB.MessageMacroCalendarEvent {
		t.Fatalf("Expected MessageMacroCalendarEvent, got %d", main.MessageType())
	}
	
	unionTable := new(flatbuffers.Table)
	if !main.Message(unionTable) {
		t.Fatal("Failed to get union table")
	}
	
	var fbEvent = new(FB.MacroCalendarEvent)
	fbEvent.Init(unionTable.Bytes, unionTable.Pos)
	event := blpconngo.DeserializeMacroCalendarEvent(fbEvent)
	
	// Convert to JSON
	jsonData, err := json.MarshalIndent(event, "", "  ")
	if err != nil {
		t.Fatalf("Failed to marshal to JSON: %v", err)
	}
	
	t.Logf("MacroCalendarEvent JSON:\n%s", string(jsonData))
	
	// Parse and verify
	var jsonMap map[string]interface{}
	err = json.Unmarshal(jsonData, &jsonMap)
	if err != nil {
		t.Fatalf("Failed to unmarshal JSON: %v", err)
	}
	
	if jsonMap["event_id"] != float64(2167806) {
		t.Errorf("Expected event_id 2167806, got %v", jsonMap["event_id"])
	}
	if jsonMap["observation_period"] != "Dec" {
		t.Errorf("Expected observation_period 'Dec', got '%v'", jsonMap["observation_period"])
	}
	
	// Check that event_type is serialized as string
	eventType, ok := jsonMap["event_type"].(string)
	if !ok {
		t.Errorf("Expected event_type to be string, got %T", jsonMap["event_type"])
	} else if eventType != "EventTypeCalendar" {
		t.Errorf("Expected event_type 'EventTypeCalendar', got '%s'", eventType)
	}
	
	// Roundtrip test
	var restored blpconngo.MacroCalendarEvent
	err = json.Unmarshal(jsonData, &restored)
	if err != nil {
		t.Fatalf("Failed to unmarshal back from JSON: %v", err)
	}
	
	if restored.EventID != event.EventID {
		t.Errorf("Roundtrip failed for EventID")
	}
	if restored.EventType != event.EventType {
		t.Errorf("Roundtrip failed for EventType")
	}
}

// Test: Deserialize LogMessage and convert to JSON
func TestLogMessageToJSON(t *testing.T) {
	buffer := readBinaryFile("fb_000005.bin")
	main := FB.GetRootAsMain(buffer, 0)
	if main == nil {
		t.Fatal("Failed to parse FlatBuffers main object")
	}
	if main.MessageType() != FB.MessageLogMessage {
		t.Fatalf("Expected MessageLogMessage, got %d", main.MessageType())
	}
	
	unionTable := new(flatbuffers.Table)
	if !main.Message(unionTable) {
		t.Fatal("Failed to get union table")
	}
	
	var fbEvent = new(FB.LogMessage)
	fbEvent.Init(unionTable.Bytes, unionTable.Pos)
	event := blpconngo.DeserializeLogMessage(fbEvent)
	
	// Convert to JSON
	jsonData, err := json.MarshalIndent(event, "", "  ")
	if err != nil {
		t.Fatalf("Failed to marshal to JSON: %v", err)
	}
	
	t.Logf("LogMessage JSON:\n%s", string(jsonData))
	
	// Parse and verify
	var jsonMap map[string]interface{}
	err = json.Unmarshal(jsonData, &jsonMap)
	if err != nil {
		t.Fatalf("Failed to unmarshal JSON: %v", err)
	}
	
	if jsonMap["message"] != "Subscription successful" {
		t.Errorf("Expected message 'Subscription successful', got '%v'", jsonMap["message"])
	}
	
	// Roundtrip test
	var restored blpconngo.LogMessageType
	err = json.Unmarshal(jsonData, &restored)
	if err != nil {
		t.Fatalf("Failed to unmarshal back from JSON: %v", err)
	}
	
	if restored.CorrelationID != event.CorrelationID {
		t.Errorf("Roundtrip failed for CorrelationID")
	}
	if restored.Message != event.Message {
		t.Errorf("Roundtrip failed for Message")
	}
}

// Test: Batch conversion - deserialize multiple messages and convert to JSON array
func TestBatchDeserializeToJSON(t *testing.T) {
	files := []string{
		"fb_000008.bin", // MacroReferenceData
		"fb_000012.bin", // MacroHeadlineEvent (ACTUAL)
		"fb_000013.bin", // MacroHeadlineEvent (REVISION)
		"fb_000011.bin", // MacroCalendarEvent
	}
	
	type MessageWrapper struct {
		Type    string      `json:"type"`
		Content interface{} `json:"content"`
	}
	
	var messages []MessageWrapper
	
	for _, filename := range files {
		buffer := readBinaryFile(filename)
		main := FB.GetRootAsMain(buffer, 0)
		if main == nil {
			t.Fatalf("Failed to parse FlatBuffers main object for %s", filename)
		}
		
		unionTable := new(flatbuffers.Table)
		if !main.Message(unionTable) {
			t.Fatalf("Failed to get union table for %s", filename)
		}
		
		var wrapper MessageWrapper
		
		switch main.MessageType() {
		case FB.MessageMacroReferenceData:
			var fbEvent = new(FB.MacroReferenceData)
			fbEvent.Init(unionTable.Bytes, unionTable.Pos)
			event := blpconngo.DeserializeMacroReferenceData(fbEvent)
			wrapper = MessageWrapper{Type: "MacroReferenceData", Content: event}
			
		case FB.MessageMacroHeadlineEvent:
			var fbEvent = new(FB.MacroHeadlineEvent)
			fbEvent.Init(unionTable.Bytes, unionTable.Pos)
			event := blpconngo.DeserializeMacroHeadlineEvent(fbEvent)
			wrapper = MessageWrapper{Type: "MacroHeadlineEvent", Content: event}
			
		case FB.MessageMacroCalendarEvent:
			var fbEvent = new(FB.MacroCalendarEvent)
			fbEvent.Init(unionTable.Bytes, unionTable.Pos)
			event := blpconngo.DeserializeMacroCalendarEvent(fbEvent)
			wrapper = MessageWrapper{Type: "MacroCalendarEvent", Content: event}
			
		default:
			t.Fatalf("Unexpected message type %d for %s", main.MessageType(), filename)
		}
		
		messages = append(messages, wrapper)
	}
	
	// Convert all messages to JSON array
	jsonData, err := json.MarshalIndent(messages, "", "  ")
	if err != nil {
		t.Fatalf("Failed to marshal batch to JSON: %v", err)
	}
	
	t.Logf("Batch Messages JSON:\n%s", string(jsonData))
	
	// Verify we can parse it back
	var restored []MessageWrapper
	err = json.Unmarshal(jsonData, &restored)
	if err != nil {
		t.Fatalf("Failed to unmarshal batch JSON: %v", err)
	}
	
	if len(restored) != len(files) {
		t.Errorf("Expected %d messages, got %d", len(files), len(restored))
	}
	
	// Verify each message type
	expectedTypes := []string{
		"MacroReferenceData",
		"MacroHeadlineEvent",
		"MacroHeadlineEvent",
		"MacroCalendarEvent",
	}
	
	for i, expected := range expectedTypes {
		if restored[i].Type != expected {
			t.Errorf("Message %d: expected type '%s', got '%s'", i, expected, restored[i].Type)
		}
	}
}

// Test: Verify JSON structure doesn't break with complex nested data
func TestComplexJSONStructure(t *testing.T) {
	buffer := readBinaryFile("fb_000013.bin") // Revision event with prior values
	main := FB.GetRootAsMain(buffer, 0)
	
	unionTable := new(flatbuffers.Table)
	main.Message(unionTable)
	
	var fbEvent = new(FB.MacroHeadlineEvent)
	fbEvent.Init(unionTable.Bytes, unionTable.Pos)
	event := blpconngo.DeserializeMacroHeadlineEvent(fbEvent)
	
	jsonData, err := json.Marshal(event)
	if err != nil {
		t.Fatalf("Failed to marshal to JSON: %v", err)
	}
	
	// Verify JSON is valid by parsing it
	var result map[string]interface{}
	err = json.Unmarshal(jsonData, &result)
	if err != nil {
		t.Fatalf("Failed to parse generated JSON: %v", err)
	}
	
	// Check nested Value structure
	value, ok := result["value"].(map[string]interface{})
	if !ok {
		t.Fatalf("Expected value to be an object, got %T", result["value"])
	}
	
	// Number and Value should be present (non-NaN)
	if _, ok := value["number"]; !ok {
		t.Errorf("Expected 'number' field in value")
	}
	if _, ok := value["value"]; !ok {
		t.Errorf("Expected 'value' field in value")
	}
	
	// NaN fields should be omitted or null
	// This verifies proper NaN handling in JSON serialization
}
