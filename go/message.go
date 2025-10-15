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
	Module        ModuleType
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

type MacroReferenceData struct {
	CorrelationID						uint64  `json:"corr_id"`
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
	CorrelationID						uint64  		`json:"corr_id"`
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
	CorrelationID						uint64  		`json:"corr_id"`
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


