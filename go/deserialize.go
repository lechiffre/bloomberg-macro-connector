package blpconngo

import (
	"blpconngo/BlpConn/FB"
	"time"
	"fmt"
)

func DeserializeDateTime(fbDateTime *FB.DateTime) time.Time {
	if fbDateTime == nil {
		// Return zero time if DateTime object is nil
		return time.Time{}
	}
	micros := fbDateTime.Micros()
	// Handle unset/invalid datetimes
	// 0 = explicitly unset
	// > MaxValidMicroseconds = invalid (overflow from negative time_t)
	if micros == 0 || !IsValidDateTime(micros) {
		// Return zero time for invalid/unset datetimes
		return time.Time{}
	}
	return ToNativeTime(micros, fbDateTime.Offset())
}

func DeserializeValue(fbValue *FB.Value) ValueType {
	if fbValue == nil {
		// Return ValueType with NaN values if fbValue is nil
		return NewValueType()
	}
	return ValueType{
		Number:            fbValue.Number(),
		Value:             fbValue.Value(),
		Low:               fbValue.Low(),
		High:              fbValue.High(),
		Median:            fbValue.Median(),
		Average:           fbValue.Average(),
		StandardDeviation: fbValue.StandardDeviation(),
	}
}

func DeserializeMacroReferenceData(fbEvent *FB.MacroReferenceData) MacroReferenceData {
	return MacroReferenceData{
		CorrelationID:				uint64(fbEvent.CorrId()),
		IDBBGlobal:        			string(fbEvent.IdBbGlobal()),
		ParsekyableDes:    			string(fbEvent.ParsekyableDes()),
		Description:       			string(fbEvent.Description()),
		IndxFreq:					string(fbEvent.IndxFreq()),
		IndxUnits:					string(fbEvent.IndxUnits()),
		CountryISO:					string(fbEvent.CountryIso()),
		IndxSource:					string(fbEvent.IndxSource()),
		SeasonalityTransformation:	string(fbEvent.SeasonalityTransformation()),
	}
}

func DeserializeMacroHeadlineEvent(fbEvent *FB.MacroHeadlineEvent) MacroHeadlineEvent {
	return MacroHeadlineEvent{
		CorrelationID:					uint64(fbEvent.CorrId()),
		EventType:         				EventType(fbEvent.EventType()),
		EventSubType:      				EventSubType(fbEvent.EventSubtype()),
		EventID:           				uint64(fbEvent.EventId()),
		ObservationPeriod: 				string(fbEvent.ObservationPeriod()),
		ReleaseStartDT:    				DeserializeDateTime(fbEvent.ReleaseStartDt(nil)),
		ReleaseEndDT:      				DeserializeDateTime(fbEvent.ReleaseEndDt(nil)),
		PriorEventID:                	uint64(fbEvent.PriorEventId()),
		PriorObservationPeriod:      	string(fbEvent.PriorObservationPeriod()),
		PriorEconomicReleaseStartDT: 	DeserializeDateTime(fbEvent.PriorEconomicReleaseStartDt(nil)),
		PriorEconomicReleaseEndDT:   	DeserializeDateTime(fbEvent.PriorEconomicReleaseEndDt(nil)),
		Value:                       	DeserializeValue(fbEvent.Value(nil)),
		PriorValue:                  	DeserializeValue(fbEvent.PriorValue(nil)),
	}
}

func DeserializeMacroCalendarEvent(fbEvent *FB.MacroCalendarEvent) MacroCalendarEvent {
	return MacroCalendarEvent{
		CorrelationID:			uint64(fbEvent.CorrId()),
		IDBBGlobal:        		string(fbEvent.IdBbGlobal()),
		ParsekyableDes:    		string(fbEvent.ParsekyableDes()),
		EventType:			   	EventType(fbEvent.EventType()),
		EventSubType:		   	EventSubType(fbEvent.EventSubtype()),
		Description:       		string(fbEvent.Description()),
		EventID:			   	uint64(fbEvent.EventId()),
		ObservationPeriod:	  	string(fbEvent.ObservationPeriod()),
		ReleaseStartDT:		   	DeserializeDateTime(fbEvent.ReleaseStartDt(nil)),
		ReleaseEndDT:		   	DeserializeDateTime(fbEvent.ReleaseEndDt(nil)),
		ReleaseStatus: 			ReleaseStatus(fbEvent.ReleaseStatus()),
		RelevanceValue:			fbEvent.RelevanceValue(),	
	}
}

func DeserializeLogMessage(fbLogMessage *FB.LogMessage) LogMessageType {
	fmt.Printf("Status: %d\n", fbLogMessage.Status())
	return LogMessageType{
		LogDT:      DeserializeDateTime(fbLogMessage.LogDt(nil)),
		Module: 	ModuleType(fbLogMessage.Module()),
		Status: 	uint8(fbLogMessage.Status()),
		CorrelationID: uint64(fbLogMessage.CorrId()),
		Message:    string(fbLogMessage.Message()),
	}
}
