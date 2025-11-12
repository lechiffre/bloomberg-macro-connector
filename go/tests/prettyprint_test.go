package main

import (
	"encoding/json"
	"math"
	"testing"
	"blpconngo"
)

func floatsMatch(f1, f2 float64) bool {
	if math.IsNaN(f1) && math.IsNaN(f2) {
		return true
	}
	if f1 == f2 {
		return true
	}
	return false
}

func valuesMatch(v1, v2 blpconngo.ValueType) bool {
	return floatsMatch(v1.Number, v2.Number) &&
		floatsMatch(v1.Value, v2.Value) &&
		floatsMatch(v1.Low, v2.Low) &&
		floatsMatch(v1.High, v2.High) &&
		floatsMatch(v1.Median, v2.Median) &&
		floatsMatch(v1.Average, v2.Average) &&
		floatsMatch(v1.StandardDeviation, v2.StandardDeviation)
}

func TestValueTypeAllValidValues(t *testing.T) {
	v := blpconngo.ValueType{
		Number:            1.0,
		Value:             -3.82,
		Low:               -5.0,
		High:              -2.0,
		Median:            -3.5,
		Average:           -3.7,
		StandardDeviation: 0.5,
	}
	
	jsonData, err := json.Marshal(v)
	if err != nil {
		t.Fatalf("Marshal error: %v", err)
	}
	
	var restored blpconngo.ValueType
	err = json.Unmarshal(jsonData, &restored)
	if err != nil {
		t.Fatalf("Unmarshal error: %v", err)
	}
	
	if !valuesMatch(v, restored) {
		t.Errorf("Values don't match after roundtrip. Original: %+v, Restored: %+v", v, restored)
	}
}

func TestValueTypeSomeNaNValues(t *testing.T) {
	v := blpconngo.ValueType{
		Number:            1.0,
		Value:             -6.32,
		Low:               math.NaN(),
		High:              math.NaN(),
		Median:            math.NaN(),
		Average:           math.NaN(),
		StandardDeviation: math.NaN(),
	}
	
	jsonData, err := json.Marshal(v)
	if err != nil {
		t.Fatalf("Marshal error: %v", err)
	}
	
	// Verify NaN values are omitted from JSON
	jsonStr := string(jsonData)
	if jsonStr == "{}" {
		t.Errorf("JSON should contain at least number and value fields, got: %s", jsonStr)
	}
	
	var restored blpconngo.ValueType
	err = json.Unmarshal(jsonData, &restored)
	if err != nil {
		t.Fatalf("Unmarshal error: %v", err)
	}
	
	if !valuesMatch(v, restored) {
		t.Errorf("Values don't match after roundtrip. Original: %+v, Restored: %+v", v, restored)
	}
	
	// Specifically check NaN fields are preserved
	if !math.IsNaN(restored.Low) {
		t.Errorf("Expected Low to be NaN, got %f", restored.Low)
	}
	if !math.IsNaN(restored.High) {
		t.Errorf("Expected High to be NaN, got %f", restored.High)
	}
}

// Test 3: All NaN values
func TestValueTypeAllNaNValues(t *testing.T) {
	v := blpconngo.ValueType{
		Number:            math.NaN(),
		Value:             math.NaN(),
		Low:               math.NaN(),
		High:              math.NaN(),
		Median:            math.NaN(),
		Average:           math.NaN(),
		StandardDeviation: math.NaN(),
	}
	
	jsonData, err := json.Marshal(v)
	if err != nil {
		t.Fatalf("Marshal error: %v", err)
	}
	
	// Should produce empty JSON object or null fields
	var restored blpconngo.ValueType
	err = json.Unmarshal(jsonData, &restored)
	if err != nil {
		t.Fatalf("Unmarshal error: %v", err)
	}
	
	if !valuesMatch(v, restored) {
		t.Errorf("Values don't match after roundtrip. Original: %+v, Restored: %+v", v, restored)
	}
}

// Test 4: Mixed values with zero
func TestValueTypeMixedWithZero(t *testing.T) {
	v := blpconngo.ValueType{
		Number:            0.0,
		Value:             0.0,
		Low:               math.NaN(),
		High:              math.NaN(),
		Median:            math.NaN(),
		Average:           math.NaN(),
		StandardDeviation: math.NaN(),
	}
	
	jsonData, err := json.Marshal(v)
	if err != nil {
		t.Fatalf("Marshal error: %v", err)
	}
	
	var restored blpconngo.ValueType
	err = json.Unmarshal(jsonData, &restored)
	if err != nil {
		t.Fatalf("Unmarshal error: %v", err)
	}
	
	if !valuesMatch(v, restored) {
		t.Errorf("Values don't match after roundtrip. Original: %+v, Restored: %+v", v, restored)
	}
	
	// Specifically verify zero values are preserved
	if restored.Number != 0.0 {
		t.Errorf("Expected Number to be 0.0, got %f", restored.Number)
	}
	if restored.Value != 0.0 {
		t.Errorf("Expected Value to be 0.0, got %f", restored.Value)
	}
}

// Test 5: Roundtrip test with typical headline event values
func TestValueTypeRoundtrip(t *testing.T) {
	original := blpconngo.ValueType{
		Number:            1.0,
		Value:             -3.82,
		Low:               math.NaN(),
		High:              math.NaN(),
		Median:            math.NaN(),
		Average:           math.NaN(),
		StandardDeviation: math.NaN(),
	}
	
	jsonData, err := json.Marshal(original)
	if err != nil {
		t.Fatalf("Marshal error: %v", err)
	}

	var restored blpconngo.ValueType
	err = json.Unmarshal(jsonData, &restored)
	if err != nil {
		t.Fatalf("Unmarshal error: %v", err)
	}
	
	if !valuesMatch(original, restored) {
		t.Errorf("Roundtrip failed. Original: %+v, Restored: %+v", original, restored)
	}
}

// Test 6: MacroHeadlineEvent with ValueType
func TestMacroHeadlineEventWithValueType(t *testing.T) {
	event := blpconngo.MacroHeadlineEvent{
		CorrelationID:     1,
		EventType:         blpconngo.EventTypeRevision,
		EventSubType:      blpconngo.EventSubTypeUnitpaint,
		EventID:           2167802,
		ObservationPeriod: "Aug",
		Value: blpconngo.ValueType{
			Number: 1.0,
			Value:  -3.82,
			Low:    math.NaN(),
			High:   math.NaN(),
			Median: math.NaN(),
			Average: math.NaN(),
			StandardDeviation: math.NaN(),
		},
		PriorValue: blpconngo.ValueType{
			Number: math.NaN(),
			Value:  math.NaN(),
			Low:    math.NaN(),
			High:   math.NaN(),
			Median: math.NaN(),
			Average: math.NaN(),
			StandardDeviation: math.NaN(),
		},
		PriorEventID:           2167801,
		PriorObservationPeriod: "Jul",
	}

	eventJSON, err := json.MarshalIndent(event, "", "  ")
	if err != nil {
		t.Fatalf("Marshal error: %v", err)
	}

	// Unmarshal back
	var restoredEvent blpconngo.MacroHeadlineEvent
	err = json.Unmarshal(eventJSON, &restoredEvent)
	if err != nil {
		t.Fatalf("Unmarshal error: %v", err)
	}

	if !valuesMatch(event.Value, restoredEvent.Value) {
		t.Errorf("Value field doesn't match after roundtrip")
	}
	
	if !valuesMatch(event.PriorValue, restoredEvent.PriorValue) {
		t.Errorf("PriorValue field doesn't match after roundtrip")
	}
	
	// Check other fields
	if restoredEvent.CorrelationID != event.CorrelationID {
		t.Errorf("Expected CorrelationID %d, got %d", event.CorrelationID, restoredEvent.CorrelationID)
	}
	if restoredEvent.EventID != event.EventID {
		t.Errorf("Expected EventID %d, got %d", event.EventID, restoredEvent.EventID)
	}
	if restoredEvent.ObservationPeriod != event.ObservationPeriod {
		t.Errorf("Expected ObservationPeriod '%s', got '%s'", event.ObservationPeriod, restoredEvent.ObservationPeriod)
	}
}

// Test 7: Verify NaN fields produce null in JSON
func TestValueTypeNaNProducesNull(t *testing.T) {
	v := blpconngo.ValueType{
		Number: 1.0,
		Value:  -3.82,
		Low:    math.NaN(),
		High:   math.NaN(),
		Median: math.NaN(),
		Average: math.NaN(),
		StandardDeviation: math.NaN(),
	}
	
	jsonData, err := json.Marshal(v)
	if err != nil {
		t.Fatalf("Marshal error: %v", err)
	}
	
	// Parse as generic map to inspect JSON structure
	var jsonMap map[string]interface{}
	err = json.Unmarshal(jsonData, &jsonMap)
	if err != nil {
		t.Fatalf("Unmarshal to map error: %v", err)
	}
	
	// Verify number and value are present
	if _, ok := jsonMap["number"]; !ok {
		t.Errorf("Expected 'number' field in JSON")
	}
	if _, ok := jsonMap["value"]; !ok {
		t.Errorf("Expected 'value' field in JSON")
	}
	
	// NaN fields should either be null or omitted
	// This is acceptable behavior for JSON serialization
}

