package main

import (
	"blpconngo"
	"testing"
)

func TestNewReferenceMap(t *testing.T) {
	refMap := blpconngo.NewReferenceMap()
	_, err := refMap.Get(999)
	if err == nil {
		t.Error("Expected error for non-existent key in newly created map")
	}
}

func TestAddAndGetReference(t *testing.T) {
	refMap := blpconngo.NewReferenceMap()
	ref := blpconngo.MacroReferenceData{
		CorrelationID:             12,
		IDBBGlobal:                "BBG002SBJ964",
		ParsekyableDes:            "CATBTOTB Index",
		Description:               "STCA Canada Merchandise Trade Total Balance SA CAD",
		IndxFreq:                  "Monthly",
		IndxUnits:                 "Value",
		CountryISO:                "CA",
		IndxSource:                "STCA - Statistics Canada",
		SeasonalityTransformation: "Value SA",
	}
	refMap.Add(ref)
	retrieved, err := refMap.Get(12)
	if err != nil {
		t.Errorf("Expected to retrieve reference, got error: %v", err)
	}
	if retrieved.CorrelationID != 12 {
		t.Errorf("Expected CorrelationID 12, got %d", retrieved.CorrelationID)
	}
	if retrieved.IDBBGlobal != "BBG002SBJ964" {
		t.Errorf("Expected IDBBGlobal 'BBG002SBJ964', got '%s'", retrieved.IDBBGlobal)
	}
	if retrieved.ParsekyableDes != "CATBTOTB Index" {
		t.Errorf("Expected ParsekyableDes 'CATBTOTB Index', got '%s'", retrieved.ParsekyableDes)
	}
	if retrieved.Description != "STCA Canada Merchandise Trade Total Balance SA CAD" {
		t.Errorf("Expected Description 'STCA Canada Merchandise Trade Total Balance SA CAD', got '%s'", retrieved.Description)
	}
	if retrieved.IndxFreq != "Monthly" {
		t.Errorf("Expected IndxFreq 'Monthly', got '%s'", retrieved.IndxFreq)
	}
	if retrieved.IndxUnits != "Value" {
		t.Errorf("Expected IndxUnits 'Value', got '%s'", retrieved.IndxUnits)
	}
	if retrieved.CountryISO != "CA" {
		t.Errorf("Expected CountryISO 'CA', got '%s'", retrieved.CountryISO)
	}
	if retrieved.IndxSource != "STCA - Statistics Canada" {
		t.Errorf("Expected IndxSource 'STCA - Statistics Canada', got '%s'", retrieved.IndxSource)
	}
	if retrieved.SeasonalityTransformation != "Value SA" {
		t.Errorf("Expected SeasonalityTransformation 'Value SA', got '%s'", retrieved.SeasonalityTransformation)
	}
}

func TestGetNonExistentReference(t *testing.T) {
	refMap := blpconngo.NewReferenceMap()
	_, err := refMap.Get(999)
	if err == nil {
		t.Error("Expected error when getting non-existent reference, got nil")
	}
}

func TestRemoveReference(t *testing.T) {
	refMap := blpconngo.NewReferenceMap()
	ref := blpconngo.MacroReferenceData{
		CorrelationID:  12,
		IDBBGlobal:     "BBG002SBJ964",
		ParsekyableDes: "CATBTOTB Index",
	}
	refMap.Add(ref)
	retrieved, err := refMap.Get(12)
	if err != nil {
		t.Errorf("Expected to retrieve reference before removal, got error: %v", err)
	}
	if retrieved.CorrelationID != 12 {
		t.Errorf("Expected CorrelationID 12, got %d", retrieved.CorrelationID)
	}
	refMap.Remove(12)
	_, err = refMap.Get(12)
	if err == nil {
		t.Error("Expected error when getting removed reference, got nil")
	}
}

func TestRemoveNonExistentReference(t *testing.T) {
	refMap := blpconngo.NewReferenceMap()
	refMap.Remove(999)
}

func TestLookAndRemoveWithSubscriptionFailure(t *testing.T) {
	refMap := blpconngo.NewReferenceMap()
	ref := blpconngo.MacroReferenceData{
		CorrelationID:  12,
		IDBBGlobal:     "BBG002SBJ964",
		ParsekyableDes: "CATBTOTB Index",
	}
	refMap.Add(ref)
	logEvent := blpconngo.LogMessageType{
		Module:        blpconngo.ModuleSubscription,
		Status:        uint8(blpconngo.SubscriptionFailure),
		CorrelationID: 12,
		Message:       "Subscription failed",
	}
	refMap.LookAndRemove(logEvent)
	_, err := refMap.Get(12)
	if err == nil {
		t.Error("Expected reference to be removed after subscription failure")
	}
}

func TestLookAndRemoveWithSubscriptionTerminated(t *testing.T) {
	refMap := blpconngo.NewReferenceMap()
	ref := blpconngo.MacroReferenceData{
		CorrelationID:  12,
		IDBBGlobal:     "BBG002SBJ964",
		ParsekyableDes: "CATBTOTB Index",
	}
	refMap.Add(ref)
	logEvent := blpconngo.LogMessageType{
		Module:        blpconngo.ModuleSubscription,
		Status:        uint8(blpconngo.SubscriptionTerminated),
		CorrelationID: 12,
		Message:       "Subscription terminated",
	}
	refMap.LookAndRemove(logEvent)
	_, err := refMap.Get(12)
	if err == nil {
		t.Error("Expected reference to be removed after subscription termination")
	}
}

func TestLookAndRemoveWithSubscriptionSuccess(t *testing.T) {
	refMap := blpconngo.NewReferenceMap()
	ref := blpconngo.MacroReferenceData{
		CorrelationID:  12,
		IDBBGlobal:     "BBG002SBJ964",
		ParsekyableDes: "CATBTOTB Index",
	}
	refMap.Add(ref)
	logEvent := blpconngo.LogMessageType{
		Module:        blpconngo.ModuleSubscription,
		Status:        uint8(blpconngo.SubscriptionSuccess),
		CorrelationID: 12,
		Message:       "Subscription successful",
	}
	refMap.LookAndRemove(logEvent)
	_, err := refMap.Get(12)
	if err != nil {
		t.Error("Expected reference to remain after subscription success")
	}
}

func TestLookAndRemoveWithNonSubscriptionModule(t *testing.T) {
	refMap := blpconngo.NewReferenceMap()
	ref := blpconngo.MacroReferenceData{
		CorrelationID:  12,
		IDBBGlobal:     "BBG002SBJ964",
		ParsekyableDes: "CATBTOTB Index",
	}
	refMap.Add(ref)
	logEvent := blpconngo.LogMessageType{
		Module:        blpconngo.ModuleSession,
		Status:        uint8(blpconngo.SessionTerminated),
		CorrelationID: 12,
		Message:       "Session terminated",
	}
	refMap.LookAndRemove(logEvent)
	_, err := refMap.Get(12)
	if err != nil {
		t.Error("Expected reference to remain for non-subscription module events")
	}
}

func TestMultipleReferences(t *testing.T) {
	refMap := blpconngo.NewReferenceMap()

	ref1 := blpconngo.MacroReferenceData{
		CorrelationID:  10,
		IDBBGlobal:     "BBG000001",
		ParsekyableDes: "INDEX1 Index",
	}

	ref2 := blpconngo.MacroReferenceData{
		CorrelationID:  20,
		IDBBGlobal:     "BBG000002",
		ParsekyableDes: "INDEX2 Index",
	}

	ref3 := blpconngo.MacroReferenceData{
		CorrelationID:  30,
		IDBBGlobal:     "BBG000003",
		ParsekyableDes: "INDEX3 Index",
	}

	refMap.Add(ref1)
	refMap.Add(ref2)
	refMap.Add(ref3)

	retrieved1, err1 := refMap.Get(10)
	if err1 != nil || retrieved1.IDBBGlobal != "BBG000001" {
		t.Error("Failed to retrieve reference 1")
	}

	retrieved2, err2 := refMap.Get(20)
	if err2 != nil || retrieved2.IDBBGlobal != "BBG000002" {
		t.Error("Failed to retrieve reference 2")
	}

	retrieved3, err3 := refMap.Get(30)
	if err3 != nil || retrieved3.IDBBGlobal != "BBG000003" {
		t.Error("Failed to retrieve reference 3")
	}

	refMap.Remove(20)

	_, err := refMap.Get(20)
	if err == nil {
		t.Error("Expected reference 2 to be removed")
	}

	_, err1 = refMap.Get(10)
	if err1 != nil {
		t.Error("Expected reference 1 to still exist")
	}

	_, err3 = refMap.Get(30)
	if err3 != nil {
		t.Error("Expected reference 3 to still exist")
	}
}

func TestUpdateExistingReference(t *testing.T) {
	refMap := blpconngo.NewReferenceMap()

	ref1 := blpconngo.MacroReferenceData{
		CorrelationID:  12,
		IDBBGlobal:     "BBG000001",
		ParsekyableDes: "INDEX1 Index",
		Description:    "Original Description",
	}

	refMap.Add(ref1)

	retrieved1, _ := refMap.Get(12)
	if retrieved1.Description != "Original Description" {
		t.Errorf("Expected Description 'Original Description', got '%s'", retrieved1.Description)
	}

	ref2 := blpconngo.MacroReferenceData{
		CorrelationID:  12,
		IDBBGlobal:     "BBG000002",
		ParsekyableDes: "INDEX2 Index",
		Description:    "Updated Description",
	}

	refMap.Add(ref2)

	retrieved2, _ := refMap.Get(12)
	if retrieved2.Description != "Updated Description" {
		t.Errorf("Expected Description 'Updated Description', got '%s'", retrieved2.Description)
	}
	if retrieved2.IDBBGlobal != "BBG000002" {
		t.Errorf("Expected IDBBGlobal 'BBG000002', got '%s'", retrieved2.IDBBGlobal)
	}
}
