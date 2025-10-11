package main

import (
	"os"
	"testing"
	"blpconngo"
	"blpconngo/BlpConn/FB"
	flatbuffers "github.com/google/flatbuffers/go"
)

const datadir = "../../data"

func readFBFile(filename string) []byte {
	filename = datadir + "/" + filename
	data, err := os.ReadFile(filename)
	if err != nil {
		panic(err)
	}
	return data
}

func TestDeserializeMacroReferenceData(t *testing.T) {
	buffer := readFBFile("fb_000008.bin")
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
	if event.CorrID != 12 {
		t.Errorf("Expected CorrID 12, got %d", event.CorrID)
	}
	if event.IDBBGlobal != "BBG002SBJ964" {
		t.Errorf("Expected IDBBGlobal 'BBG002SBJ964', got '%s'", event.IDBBGlobal)
	}
	if event.ParsekyableDes != "CATBTOTB Index" {
		t.Errorf("Expected ParsekyableDes 'CATBTOTB Index', got '%s'", event.ParsekyableDes)
	}
	if event.Description != "STCA Canada Merchandise Trade Total Balance SA CAD" {
		t.Errorf("Expected Description 'STCA Canada Merchandise Trade Total Balance SA CAD', got '%s'", event.Description)
	}
	if event.IndxFreq != "Monthly" {
		t.Errorf("Expected IndxFreq 'Monthly', got '%s'", event.IndxFreq)
	}
	if event.IndxUnits != "Value" {
		t.Errorf("Expected IndxUnits 'Value', got '%s'", event.IndxUnits)
	}
	if event.CountryISO != "CA" {
		t.Errorf("Expected CountryISO 'CA', got '%s'", event.CountryISO)
	}
	if event.IndxSource != "STCA - Statistics Canada" {
		t.Errorf("Expected IndxSource 'STCA - Statistics Canada', got '%s'", event.IndxSource)
	}
	if event.SeasonalityTransformation != "Value SA" {
		t.Errorf("Expected SeasonalityTransformation 'Value SA', got '%s'", event.SeasonalityTransformation)
	}
}

func TestDeserializeMacroHeadlineEvent(t *testing.T) {
	buffer := readFBFile("fb_000014.bin")
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
	if event.CorrID != 12 {
		t.Errorf("Expected CorrID 12, got %d", event.CorrID)
	}
	if event.EventType != blpconngo.EventTypeRevision {
		t.Errorf("Expected EventType Revision, got %d", event.EventType)
	}
	if event.EventSubType != blpconngo.EventSubTypeUnitpaint {
		t.Errorf("Expected EventSubType Unitpaint, got %d", event.EventSubType)
	}
	if event.EventID != 2167802 {
		t.Errorf("Expected EventID 2167802, got %d", event.EventID)
	}
	if event.ObservationPeriod != "Aug" {
		t.Errorf("Expected ObservationPeriod 'Aug', got '%s'", event.ObservationPeriod)
	}
	/*
	if event.ReleaseStartDT.Micros() != 1759840200000000 {
		t.Errorf("Expected ReleaseStartDT.Micros() 1759840200000000, got %d", event.ReleaseStartDT.Micros())
	}
	if event.ReleaseStartDT.Offset() != 0 {
		t.Errorf("Expected ReleaseStartDT.Offset() 0, got %d", event.ReleaseStartDT.Offset())
	}
	if event.ReleaseEndDT.Micros() != 1759840200000000 {
		t.Errorf("Expected ReleaseEndDT.Micros() 1759840200000000, got %d", event.ReleaseEndDT.Micros())
	}
	if event.ReleaseEndDT.Offset() != 0 {
		t.Errorf("Expected ReleaseEndDT.Offset() 0, got %d", event.ReleaseEndDT.Offset())
	}
	*/
	if event.PriorEventID != 2167801 {
		t.Errorf("Expected PriorEventID 2167801, got %d", event.PriorEventID)
	}
	if event.PriorObservationPeriod != "Jul" {
		t.Errorf("Expected PriorObservationPeriod 'Jul', got '%s'", event.PriorObservationPeriod)
	}
	/*
	if event.PriorEconomicReleaseStartDT.Micros() != 1756989000000000 {
		t.Errorf("Expected PriorEconomicReleaseStartDT.Micros() 1756989000000000, got %d", event.PriorEconomicReleaseStartDT.Micros())
	}
	if event.PriorEconomicReleaseStartDT.Offset() != 0 {
		t.Errorf("Expected PriorEconomicReleaseStartDT.Offset() 0, got %d", event.PriorEconomicReleaseStartDT.Offset())
	}
	if event.PriorEconomicReleaseEndDT.Micros() != 1756989000000000 {
		t.Errorf("Expected PriorEconomicReleaseEndDT.Micros() 1756989000000000, got %d", event.PriorEconomicReleaseEndDT.Micros())
	}
	if event.PriorEconomicReleaseEndDT.Offset() != 0 {
		t.Errorf("Expected PriorEconomicReleaseEndDT.Offset() 0, got %d", event.PriorEconomicReleaseEndDT.Offset())
	}
	*/
	if event.Value.Number != 1.0 {
		t.Errorf("Expected Value.Number 1, got %f", event.Value.Number)
	}
	if event.Value.Value != -3.82 {
		t.Errorf("Expected Value.Value -3.82, got %f", event.Value.Value)
	}
	if !isNaN(event.Value.Low) {
		t.Errorf("Expected Value.Low to be NaN, got %f", event.Value.Low)
	}
	if !isNaN(event.Value.High) {
		t.Errorf("Expected Value.High to be NaN, got %f", event.Value.High)
	}
	if !isNaN(event.Value.Median) {
		t.Errorf("Expected Value.Median to be NaN, got %f", event.Value.Median)
	}
	if !isNaN(event.Value.Average) {
		t.Errorf("Expected Value.Average to be NaN, got %f", event.Value.Average)
	}
	if !isNaN(event.Value.StandardDeviation) {
		t.Errorf("Expected Value.StandardDeviation to be NaN, got %f", event.Value.StandardDeviation)
	}
}

func TestDeserializeMacroCalendarEvent(t *testing.T) {
	buffer := readFBFile("fb_000012.bin")
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
	if event.CorrID != 12 {
		t.Errorf("Expected CorrID 12, got %d", event.CorrID)
	}
	if event.IDBBGlobal != "" {
		t.Errorf("Expected IDBBGlobal '', got '%s'", event.IDBBGlobal)
	}
	if event.ParsekyableDes != "" {
		t.Errorf("Expected ParsekyableDes '', got '%s'", event.ParsekyableDes)
	}
	if event.EventType != blpconngo.EventTypeCalendar {
		t.Errorf("Expected EventType Calendar, got %d", event.EventType)
	}
	if event.EventSubType != blpconngo.EventSubTypeUnitpaint {
		t.Errorf("Expected EventSubType Unitpaint, got %d", event.EventSubType)
	}
	if event.Description != "" {
		t.Errorf("Expected Description '', got '%s'", event.Description)
	}
	if event.EventID != 2167806 {
		t.Errorf("Expected EventID 2167806, got %d", event.EventID)
	}
	if event.ObservationPeriod != "Dec" {
		t.Errorf("Expected ObservationPeriod 'Dec', got '%s'", event.ObservationPeriod)
	}
	/*
	if event.ReleaseStartDT.Micros() != 1770298200000000 {
		t.Errorf("Expected ReleaseStartDT.Micros() 1770298200000000, got %d", event.ReleaseStartDT.Micros())
	}
	if event.ReleaseStartDT.Offset() != 0 {
		t.Errorf("Expected ReleaseStartDT.Offset() 0, got %d", event.ReleaseStartDT.Offset())
	}
	if event.ReleaseEndDT.Micros() != 1770298200000000 {
		t.Errorf("Expected ReleaseEndDT.Micros() 1770298200000000, got %d", event.ReleaseEndDT.Micros())
	}
	if event.ReleaseEndDT.Offset() != 0 {
		t.Errorf("Expected ReleaseEndDT.Offset() 0, got %d", event.ReleaseEndDT.Offset())
	}
	if event.ReleaseStatus != blpconngo.ReleaseStatusScheduled {
		t.Errorf("Expected ReleaseStatus Scheduled, got %d", event.ReleaseStatus)
	}
	*/
	if event.RelevanceValue != 55.2632 {
		t.Errorf("Expected RelevanceValue 55.2632, got %f", event.RelevanceValue)
	}
}

func isNaN(f float64) bool {
	return f != f
}

