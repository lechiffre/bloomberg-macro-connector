package blpconngo

import (
	"blpconngo/BlpConn/FB"
	"time"
)

func DeserializeDateTime(fbDateTime *FB.DateTime) time.Time {
	return ToNativeTime(fbDateTime.Micros(), fbDateTime.Offset())
}

func DeserializeValue(fbValue *FB.Value) ValueType {
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
	return LogMessageType{
		LogDT:      DeserializeDateTime(fbLogMessage.LogDt(nil)),
		Module: 	ModuleType(fbLogMessage.Module()),
		Status: 	fbLogMessage.Status(),
		Message:    string(fbLogMessage.Message()),
	}
}
