package blpconngo

import (
	"time"
)

const (
	ReleaseStatusUnknown int = iota
	ReleaseStatusReleased
	ReleaseStatusScheduled
	ReleaseStatusAnother = 99
)

const (
	EventSubTypeUnknown int = iota
	EventSubTypeNew
	EventSubTypeUpdate
	EventSubTypeUnitpaint
	EventSubTypeDelete
	EventSubTypeAnother = 99
)

const (
	EventTypeUnknown int = iota
	EventTypeActual
	EventTypeRevision
	EventTypeEstimate
	EventTypeCalendar
	EventTypeAnother = 99
)

const (
    ModuleUnknown int = iota
    ModuleSystem
    ModuleSession
    ModuleSubscription
    ModuleService
    ModuleHearbeat
    ModuleAnother = 99
)

const (
    SessionUnknown int = iota
    SessionConnectionUp
    SessionStarted
    SessionConnectionDown
    SessionTerminated
    SessionAnother = 99
)

type LogMessageType struct {
	LogDT      time.Time
	Module int32
	Status	   int32
	Message    string
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

type HeadlineBaseEvent struct {
	IDBBGlobal        string
	ParsekyableDes    string
	Description       string
	EventType         int
	EventSubType      int
	EventID           int
	ObservationPeriod string
	ReleaseStartDT    time.Time
	ReleaseEndDT      time.Time
}

type HeadlineEconomicEvent struct {
	HeadlineBaseEvent
	Value                       ValueType
	PriorValue                  ValueType
	PriorEventID                int
	PriorObservationPeriod      string
	PriorEconomicReleaseStartDT time.Time
	PriorEconomicReleaseEndDT   time.Time
}

type HeadlineCalendarEvent struct {
	HeadlineBaseEvent
	Status int
}

func ToNativeTime(microseconds uint64, offset int16) time.Time {
	seconds := int64(microseconds / 1e6)
	nanoseconds := int64((microseconds % 1e6) * 1e3)
	location := time.FixedZone("UTC", int(offset)*60)
	return time.Unix(seconds, nanoseconds).In(location)
}
