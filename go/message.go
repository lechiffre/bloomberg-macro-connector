package blpconngo

import (
	"math"
	"time"
)

type ModuleType uint8

const (
	ModuleUnknown ModuleType = iota
	ModuleSystem
	ModuleSession
	ModuleSubscription
	ModuleService
	ModuleHeartbeat
	ModuleAnother = 99
)

type SessionStatus uint8

const (
	SessionUnknown SessionStatus = iota
	SessionConnectionUp
	SessionStarted
	SessionConnectionDown
	SessionTerminated
	SessionInvalidOptions
	SessionFailure
	SessionAnother = 99
)

type SubscriptionStatus uint8

const (
	SubscriptionUnknown SubscriptionStatus = iota
	SubscriptionStarted
	SubscriptionStreamsActivated
	SubscriptionTerminated
	SubscriptionSuccess
	SubscriptionFailure
	SubscriptionAnother = 99
)

type ServiceStatus uint8

const (
	ServiceUnknown ServiceStatus = iota
	ServiceOpened
	ServiceClosed
	ServiceFailure
	ServiceAnother = 99
)

type ReleaseStatus uint8

const (
	ReleaseStatusUnknown ReleaseStatus = iota
	ReleaseStatusReleased
	ReleaseStatusScheduled
	ReleaseStatusAnother = 99
)

type EventSubType uint8

const (
	EventSubTypeUnknown EventSubType = iota
	EventSubTypeNew
	EventSubTypeUpdate
	EventSubTypeUnitpaint
	EventSubTypeDelete
	EventSubTypeAnother = 99
)

type EventType uint8

const (
	EventTypeUnknown EventType = iota
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

type HeadlineCalendarEvent struct {
	HeadlineBaseEvent
	ReleaseStatus uint8 
}

type MacroReferenceData struct {
	CorrID						int64  `json:"corr_id"`
	IDBBGlobal					string  `json:"id_bb_global"`
	ParsekyableDes    			string	`json:"parsekyable_des"`
	Description       			string  `json:"description"`
	IndxFreq		  			string	`json:"indx_freq"`
	IndxUnits		  			string	`json:"indx_units"`
	CountryISO		  			string	`json:"country_iso"`
	IndxSource	      			string	`json:"indx_source"`
	SeasonalityTransformation	string	`json:"seasonality_transformation"`
}

type MacroHeadlineEvent struct {
	CorrID						int64  		`json:"corr_id"`
	EventType         			EventType		`json:"event_type"`
	EventSubType      			EventSubType	`json:"event_subtype"`
	EventID           			uint64			`json:"event_id"`
	ObservationPeriod 			string			`json:"observation_period"`
	ReleaseStartDT    			time.Time		`json:"release_start_dt"`
	ReleaseEndDT      			time.Time		`json:"release_end_dt"`
	PriorEventID                uint64			`json:"prior_event_id"`
	PriorObservationPeriod      string			`json:"prior_observation_period"`
	PriorEconomicReleaseStartDT time.Time		`json:"prior_economic_release_start_dt"`
	PriorEconomicReleaseEndDT   time.Time		`json:"prior_economic_release_end_dt"`
	Value                       ValueType		`json:"value"`
}

type MacroCalendarEvent struct {
	CorrID						int64  		`json:"corr_id"`
	IDBBGlobal					string  		`json:"id_bb_global"`
	ParsekyableDes    			string 			`json:"parsekyable_des"`
	EventType         			EventType		`json:"event_type"`
	EventSubType      			EventSubType    `json:"event_subtype"`
	Description       			string  `json:"description"`
	EventID           			uint64			`json:"event_id"`
	ObservationPeriod 			string			`json:"observation_period"`
	ReleaseStartDT    			time.Time		`json:"release_start_dt"`
	ReleaseEndDT      			time.Time		`json:"release_end_dt"`
	ReleaseStatus				ReleaseStatus	`json:"release_status"`
	RelevanceValue				float64			`json:"relevance_value"`
}

type HeadlineEvent struct {
	MacroHeadlineEvent
	IDBBGlobal					string  `json:"id_bb_global"`
	ParsekyableDes    			string	`json:"parsekyable_des"`
	Description       			string  `json:"description"`
	IndxFreq		  			string	`json:"indx_freq"`
	IndxUnits		  			string	`json:"indx_units"`
	CountryISO		  			string	`json:"country_iso"`
	IndxSource	      			string	`json:"indx_source"`
	SeasonalityTransformation	string	`json:"seasonality_transformation"`
}

type CalendarEvent struct {
	MacroCalendarEvent
	Description       			string  `json:"description"`
	IndxFreq		  			string	`json:"indx_freq"`
	IndxUnits		  			string	`json:"indx_units"`
	CountryISO		  			string	`json:"country_iso"`
	IndxSource	      			string	`json:"indx_source"`
	SeasonalityTransformation	string	`json:"seasonality_transformation"`
}

func ToNativeTime(microseconds uint64, offset int16) time.Time {
	seconds := int64(microseconds / 1e6)
	nanoseconds := int64((microseconds % 1e6) * 1e3)
	location := time.FixedZone("UTC", int(offset)*60)
	return time.Unix(seconds, nanoseconds).In(location)
}

type ReferenceMap struct {
	items map[int64]MacroReferenceData
}

func NewReferenceMap() ReferenceMap {
	return ReferenceMap{
		items: make(map[int64]MacroReferenceData),
	}
}

func (refMap ReferenceMap) Add(ref MacroReferenceData) {
	refMap.items[ref.CorrID] = ref
}

func (refMap ReferenceMap) fillHeadlineEvent(event MacroHeadlineEvent) HeadlineEvent {
	ref, ok := refMap.items[event.CorrID] 
	if !ok {
		return HeadlineEvent{
			MacroHeadlineEvent: event,
			IDBBGlobal: "",
			ParsekyableDes: "",
			Description: "",
			IndxFreq: "",
			IndxUnits: "",
			CountryISO: "",
			IndxSource: "",
			SeasonalityTransformation: "",
		}
	}
	return HeadlineEvent{
		MacroHeadlineEvent: event,
		IDBBGlobal: ref.IDBBGlobal,
		ParsekyableDes: ref.ParsekyableDes,
		Description: ref.Description,
		IndxFreq: ref.IndxFreq,
		IndxUnits: ref.IndxUnits,
		CountryISO: ref.CountryISO,
		IndxSource: ref.IndxSource,
		SeasonalityTransformation: ref.SeasonalityTransformation,
	}
}

func (refMap ReferenceMap) fillCalendarEvent(event MacroCalendarEvent) CalendarEvent {
	ref, ok := refMap.items[event.CorrID] 
	var result CalendarEvent
	if !ok {
		result = CalendarEvent{
			MacroCalendarEvent: event,
			Description: "",
			IndxFreq: "",
			IndxUnits: "",
			CountryISO: "",
			IndxSource: "",
			SeasonalityTransformation: "",
		}
	} else {
		result = CalendarEvent{
			MacroCalendarEvent: event,
			Description: ref.Description,
			IndxFreq: ref.IndxFreq,
			IndxUnits: ref.IndxUnits,
			CountryISO: ref.CountryISO,
			IndxSource: ref.IndxSource,
			SeasonalityTransformation: ref.SeasonalityTransformation,
		}
	}
	result.IDBBGlobal = ref.IDBBGlobal
	result.ParsekyableDes = ref.ParsekyableDes
	return result
}
