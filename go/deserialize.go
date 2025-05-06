package main

import (
	"gocli/BlpConn/FB"
	"time"
)

func DeserializeDateTime(fbDateTime *FB.DateTime) time.Time {
	// fmt.Println(fbDateTime.Micros())
	// return ToNativeTime(fbDateTime.Micros(), fbDateTime.Offset())
	return time.Time{}
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

func DeserializeHeadlineEconomicEvent(fbEvent *FB.HeadlineEconomicEvent) HeadlineEconomicEvent {
	return HeadlineEconomicEvent{
		HeadlineBaseEvent: HeadlineBaseEvent{
			IDBBGlobal:        string(fbEvent.IdBbGlobal()),
			ParsekyableDes:    string(fbEvent.ParsekyableDes()),
			Description:       string(fbEvent.Description()),
			EventType:         int(fbEvent.EventType()),
			EventSubType:      int(fbEvent.EventSubtype()),
			EventID:           int(fbEvent.EventId()),
			ObservationPeriod: string(fbEvent.ObservationPeriod()),
			ReleaseStartDT:    DeserializeDateTime(fbEvent.ReleaseStartDt(nil)),
			ReleaseEndDT:      DeserializeDateTime(fbEvent.ReleaseEndDt(nil)),
		},
		Value:                       DeserializeValue(fbEvent.Value(nil)),
		PriorValue:                  DeserializeValue(fbEvent.PriorValue(nil)),
		PriorEventID:                int(fbEvent.PriorEventId()),
		PriorObservationPeriod:      string(fbEvent.PriorObservationPeriod()),
		PriorEconomicReleaseStartDT: DeserializeDateTime(fbEvent.PriorEconomicReleaseStartDt(nil)),
		PriorEconomicReleaseEndDT:   DeserializeDateTime(fbEvent.PriorEconomicReleaseEndDt(nil)),
	}
}

func DeserializeHeadlineCalendarEvent(fbEvent *FB.HeadlineCalendarEvent) HeadlineCalendarEvent {
	return HeadlineCalendarEvent{
		HeadlineBaseEvent: HeadlineBaseEvent{
			IDBBGlobal:        string(fbEvent.IdBbGlobal()),
			ParsekyableDes:    string(fbEvent.ParsekyableDes()),
			Description:       string(fbEvent.Description()),
			EventType:         int(fbEvent.EventType()),
			EventSubType:      int(fbEvent.EventSubtype()),
			EventID:           int(fbEvent.EventId()),
			ObservationPeriod: string(fbEvent.ObservationPeriod()),
			ReleaseStartDT:    DeserializeDateTime(fbEvent.ReleaseStartDt(nil)),
			ReleaseEndDT:      DeserializeDateTime(fbEvent.ReleaseEndDt(nil)),
		},
		Status: int(fbEvent.ReleaseStatus()),
	}
}

func DeserializeLogMessage(fbLogMessage *FB.LogMessage) LogMessageType {
	return LogMessageType{
		LogDT:      DeserializeDateTime(fbLogMessage.LogDt(nil)),
		ModuleName: string(fbLogMessage.ModuleName()),
		Message:    string(fbLogMessage.Message()),
	}
}
