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
	if event.CorrelationID != 12 {
		t.Errorf("Expected CorrelationID 12, got %d", event.CorrelationID)
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
	if event.CorrelationID != 12 {
		t.Errorf("Expected CorrelationID 12, got %d", event.CorrelationID)
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
	if event.PriorEventID != 2167801 {
		t.Errorf("Expected PriorEventID 2167801, got %d", event.PriorEventID)
	}
	if event.PriorObservationPeriod != "Jul" {
		t.Errorf("Expected PriorObservationPeriod 'Jul', got '%s'", event.PriorObservationPeriod)
	}
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
	if event.CorrelationID != 12 {
		t.Errorf("Expected CorrelationID 12, got %d", event.CorrelationID)
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
	if event.RelevanceValue != 55.2632 {
		t.Errorf("Expected RelevanceValue 55.2632, got %f", event.RelevanceValue)
	}
}

func TestDeserializeSubscriptionSuccess(t *testing.T) {
	buffer := readFBFile("fb_000005.bin")
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
	if event.CorrelationID != 12 {
		t.Errorf("Expected CorrelationID 12, got %d", event.CorrelationID)
	}
	if event.Module != blpconngo.ModuleSubscription {
		t.Errorf("Expected Module Subscription, got %d", event.Module)
	}
	if blpconngo.SubscriptionStatus(event.Status) != blpconngo.SubscriptionSuccess {
		t.Errorf("Expected Status Success, got %d", event.Status)
	}
	if event.Message != "Subscription successful" {
		t.Errorf("Expected Message 'Subscription successful', got '%s'", event.Message)
	}
}

// TEST(Deserialize, SubscriptionStreamsActivated) {
//     auto buffer = readFBFile("fb_000007.bin");
//     flatbuffers::Verifier verifier(buffer.data(), buffer.size());
//     EXPECT_TRUE(BlpConn::FB::VerifyMessageVector(verifier, nullptr, nullptr));
//     auto main = flatbuffers::GetRoot<BlpConn::FB::Main>(buffer.data());
//     EXPECT_TRUE(main->message_type() == BlpConn::FB::Message_LogMessage);
//     auto fb_data = main->message_as_LogMessage();
//     auto data = toLogMessage(fb_data);
//     EXPECT_EQ(static_cast<Module>(data.module), Module::Subscription);
//     EXPECT_EQ(static_cast<SubscriptionStatus>(data.status), SubscriptionStatus::StreamsActivated);
//     EXPECT_EQ(data.correlation_id, 12);
// }

func TestDeserializeSubscriptionStreamsActivated(t *testing.T) {
	buffer := readFBFile("fb_000007.bin")
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
	if event.CorrelationID != 12 {
		t.Errorf("Expected CorrelationID 12, got %d", event.CorrelationID)
	}
	if event.Module != blpconngo.ModuleSubscription {
		t.Errorf("Expected Module Subscription, got %d", event.Module)
	}
	if blpconngo.SubscriptionStatus(event.Status) != blpconngo.SubscriptionStreamsActivated {
		t.Errorf("Expected Status StreamsActivated, got %d", event.Status)
	}
}

// TEST(Deserialize, SubscriptionTerminated) {
//     auto buffer = readFBFile("fb_000015.bin");
//     flatbuffers::Verifier verifier(buffer.data(), buffer.size());
//     EXPECT_TRUE(BlpConn::FB::VerifyMessageVector(verifier, nullptr, nullptr));
//     auto main = flatbuffers::GetRoot<BlpConn::FB::Main>(buffer.data());
//     EXPECT_TRUE(main->message_type() == BlpConn::FB::Message_LogMessage);
//     auto fb_data = main->message_as_LogMessage();
//     auto data = toLogMessage(fb_data);
//     EXPECT_EQ(static_cast<Module>(data.module), Module::Subscription);
//     EXPECT_EQ(static_cast<SubscriptionStatus>(data.status), SubscriptionStatus::Terminated);
//     EXPECT_EQ(data.correlation_id, 12);
//     EXPECT_EQ(data.message, "SubscriptionTerminated = { reason = { source = \"SubscriptionManager\" category = \"CANCELLED\" errorCode = 0 description = \"Subscription cancelled\" } }");
// }

func TestDeserializeSubscriptionTerminated(t *testing.T) {
	buffer := readFBFile("fb_000015.bin")
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
	if event.CorrelationID != 12 {
		t.Errorf("Expected CorrelationID 12, got %d", event.CorrelationID)
	}
	if event.Module != blpconngo.ModuleSubscription {
		t.Errorf("Expected Module Subscription, got %d", event.Module)
	}
	if blpconngo.SubscriptionStatus(event.Status) != blpconngo.SubscriptionTerminated {
		t.Errorf("Expected Status Terminated, got %d", event.Status)
	}
	expectedMessage := "SubscriptionTerminated = { reason = { source = \"SubscriptionManager\" category = \"CANCELLED\" errorCode = 0 description = \"Subscription cancelled\" } }"
	if event.Message != expectedMessage {
		t.Errorf("Expected Message '%s', got '%s'", expectedMessage, event.Message)
	}
}

func isNaN(f float64) bool {
	return f != f
}

