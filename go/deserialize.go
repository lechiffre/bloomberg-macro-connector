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

func DeserializeHeadlineEconomicEvent(fbEvent *FB.HeadlineEconomicEvent) HeadlineEconomicEvent {
	return HeadlineEconomicEvent{
		HeadlineBaseEvent: HeadlineBaseEvent{
			IDBBGlobal:        string(fbEvent.IdBbGlobal()),
			ParsekyableDes:    string(fbEvent.ParsekyableDes()),
			Description:       string(fbEvent.Description()),
			EventType:         uint8(fbEvent.EventType()),
			EventSubType:      uint8(fbEvent.EventSubtype()),
			EventID:           uint64(fbEvent.EventId()),
			ObservationPeriod: string(fbEvent.ObservationPeriod()),
			ReleaseStartDT:    DeserializeDateTime(fbEvent.ReleaseStartDt(nil)),
			ReleaseEndDT:      DeserializeDateTime(fbEvent.ReleaseEndDt(nil)),
		},
		Value:                       DeserializeValue(fbEvent.Value(nil)),
		PriorValue:                  DeserializeValue(fbEvent.PriorValue(nil)),
		PriorEventID:                uint64(fbEvent.PriorEventId()),
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
			EventType:         uint8(fbEvent.EventType()),
			EventSubType:      uint8(fbEvent.EventSubtype()),
			EventID:           uint64(fbEvent.EventId()),
			ObservationPeriod: string(fbEvent.ObservationPeriod()),
			ReleaseStartDT:    DeserializeDateTime(fbEvent.ReleaseStartDt(nil)),
			ReleaseEndDT:      DeserializeDateTime(fbEvent.ReleaseEndDt(nil)),
		},
		ReleaseStatus: uint8(fbEvent.ReleaseStatus()),
	}
}

func DeserializeLogMessage(fbLogMessage *FB.LogMessage) LogMessageType {
	return LogMessageType{
		LogDT:      DeserializeDateTime(fbLogMessage.LogDt(nil)),
		Module: fbLogMessage.Module(),
		Status: fbLogMessage.Status(),
		Message:    string(fbLogMessage.Message()),
	}
}
