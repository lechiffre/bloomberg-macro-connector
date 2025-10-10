package blpconngo

import (
	"math"
	"time"
)

const (
	ModuleUnknown uint8 = iota
	ModuleSystem
	ModuleSession
	ModuleSubscription
	ModuleService
	ModuleHeartbeat
	ModuleAnother = 99
)


const (
	SessionUnknown uint8 = iota
	SessionConnectionUp
	SessionStarted
	SessionConnectionDown
	SessionTerminated
	SessionInvalidOptions
	SessionFailure
	SessionAnother = 99
)

const (
	SubscriptionUnknown uint8 = iota
	SubscriptionStarted
	SubscriptionStreamsActivated
	SubscriptionTerminated
	SubscriptionSuccess
	SubscriptionFailure
	SubscriptionAnother = 99
)

const (
	ServiceUnknown uint8 = iota
	ServiceOpened
	ServiceClosed
	ServiceFailure
	ServiceAnother = 99
)

const (
	ReleaseStatusUnknown uint8 = iota
	ReleaseStatusReleased
	ReleaseStatusScheduled
	ReleaseStatusAnother = 99
)

const (
	EventSubTypeUnknown uint8 = iota
	EventSubTypeNew
	EventSubTypeUpdate
	EventSubTypeUnitpaint
	EventSubTypeDelete
	EventSubTypeAnother = 99
)

const (
	EventTypeUnknown uint8 = iota
	EventTypeActual
	EventTypeRevision
	EventTypeEstimate
	EventTypeCalendar
	EventTypeAnother = 99
)

type DateTimeType struct {
	Microseconds uint64
	Offset       int16
}

type LogMessageType struct {
	LogDT         time.Time
	Module        uint8
	Status        uint8
	CorrelationID uint64
	Message       string
}

type ValueType struct {
	Number            float64
	Value             float64
	Low               float64
	High              float64
	Median            float64
	Average           float64
	StandardDeviation float64
}

func NewValueType() ValueType {
	return ValueType{
		Number:            math.NaN(),
		Value:             math.NaN(),
		Low:               math.NaN(),
		High:              math.NaN(),
		Median:            math.NaN(),
		Average:           math.NaN(),
		StandardDeviation: math.NaN(),
	}
}

type HeadlineBaseEvent struct {
	IDBBGlobal        string
	ParsekyableDes    string
	Description       string
	EventType         uint8
	EventSubType      uint8
	EventID           uint64
	ObservationPeriod string
	ReleaseStartDT    time.Time
	ReleaseEndDT      time.Time
}

type HeadlineEconomicEvent struct {
	HeadlineBaseEvent
	Value                       ValueType
	PriorValue                  ValueType
	PriorEventID                uint64
	PriorObservationPeriod      string
	PriorEconomicReleaseStartDT time.Time
	PriorEconomicReleaseEndDT   time.Time
}

type MacroReferenceData struct {
	IDBBGlobal					string
	ParsekyableDes    			string
	Description       			string
	IndxFreq		  			string
	IndxUnits		  			string
	CountryIso		  			string
	IndxSource	      			string
	SeasonalityTransformation	string
}

type MacroHeadlineEvent struct {
	EventType         			uint8
	EventSubType      			uint8
	EventID           			uint64
	ObservationPeriod 			string
	ReleaseStartDT    			time.Time
	ReleaseEndDT      			time.Time
	PriorEventID                uint64
	PriorObservationPeriod      string
	PriorEconomicReleaseStartDT time.Time
	PriorEconomicReleaseEndDT   time.Time
	Value                       ValueType
}

type MacroCalendarEvent struct {
	IDBBGlobal					string
	ParsekyableDes    			string
	EventType         			uint8
	EventSubType      			uint8
	Description       			string
	EventID           			uint64
	ObservationPeriod 			string
	ReleaseStartDT    			time.Time
	ReleaseEndDT      			time.Time
	ReleaseStatus 				uint8 
	RelevanceValue				float64
}

type HeadlineCalendarEvent struct {
	HeadlineBaseEvent
	ReleaseStatus uint8 
}

func ToNativeTime(microseconds uint64, offset int16) time.Time {
	seconds := int64(microseconds / 1e6)
	nanoseconds := int64((microseconds % 1e6) * 1e3)
	location := time.FixedZone("UTC", int(offset)*60)
	return time.Unix(seconds, nanoseconds).In(location)
}
