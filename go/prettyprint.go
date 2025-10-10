package blpconngo

import (
	"encoding/json"
	"math"
	"fmt"
)

// A helper struct to manage json ser/des
type PValueType struct {
	Number            *float64	`json:"number"`
	Value             *float64	`json:"value"`
	Low               *float64	`json:"low"`
	High              *float64	`json:"high"`
	Median            *float64	`json:"median"`
	Average           *float64	`json:"average"`
	StandardDeviation *float64	`json:"standard_deviation"`
}

func safePtr(f float64) *float64 {
	if math.IsNaN(f) || math.IsInf(f, 0) {
		return nil
	}
	return &f
}

func safeFloat(p *float64) float64 {
	if p == nil {
		return math.NaN()
	}
	return *p
}

func (v ValueType) MarshalJSON() ([]byte, error) {
	fmt.Println(v)
	alt := PValueType{
		Number:		safePtr(v.Number),
		Value:		safePtr(v.Value),
		Low:		safePtr(v.Low),
		High:		safePtr(v.High),
		Median:		safePtr(v.Median),
		Average:	safePtr(v.Average),
		StandardDeviation: safePtr(v.StandardDeviation),
	}
	fmt.Println(alt)
	return json.Marshal(alt)
}

func (v *ValueType) UnmarshalJSON(data []byte) error {
	var tmp PValueType
	if err := json.Unmarshal(data, &tmp); err != nil {
		return err
	}
	v.Number = safeFloat(tmp.Number)
	v.Value = safeFloat(tmp.Value)
	v.Low = safeFloat(tmp.Low)
	v.High = safeFloat(tmp.High)
	v.Median = safeFloat(tmp.Median)
	v.Average = safeFloat(tmp.Average)
	v.StandardDeviation = safeFloat(tmp.StandardDeviation)
	return nil
}

func (v ReleaseStatus) MarshalJSON() ([]byte, error) {
	return json.Marshal(v.String())
}

func (v *ReleaseStatus) UnmarshalJSON(data []byte) error {
	var s string
	if err := json.Unmarshal(data, &s); err != nil {
		return err
	}
	switch s {
		case "ReleaseStatusUnknown":
			*v = ReleaseStatusUnknown
		case "ReleaseStatusReleased":
			*v = ReleaseStatusReleased
		case "ReleaseStatusScheduled":
			*v = ReleaseStatusScheduled
		case "ReleaseStatusAnother":
			*v = ReleaseStatusAnother 
		default:
			*v = ReleaseStatusUnknown
	}
	return nil
}

func (v EventType) MarshalJSON() ([]byte, error) {
	return json.Marshal(v.String())
}

func (v *EventType) UnmarshalJSON(data []byte) error {
	var s string
	if err := json.Unmarshal(data, &s); err != nil {
		return err
	}
	switch s {
	case "EventTypeUnknown":
		*v = EventTypeUnknown
	case "EventTypeActual":
		*v = EventTypeActual
	case "EventTypeRevision":
		*v = EventTypeRevision
	case "EventTypeEstimate":
		*v = EventTypeEstimate
	case "EventTypeCalendar":
		*v = EventTypeCalendar
	case "EventTypeAnother":
		*v = EventTypeAnother
	default:
		*v = EventTypeUnknown
	}
	return nil
}

func (v EventSubType) MarshalJSON() ([]byte, error) {
	return json.Marshal(v.String())
}

func (v *EventSubType) UnmarshalJSON(data []byte) error {
	var s string
	if err := json.Unmarshal(data, &s); err != nil {
		return err
	}
	switch s {
	case "EventSubTypeUnknown ":
		*v = EventSubTypeUnknown 
	case "EventSubTypeNew":
		*v = EventSubTypeNew
	case "EventSubTypeUpdate":
		*v = EventSubTypeUpdate
	case "EventSubTypeUnitpaint":
		*v = EventSubTypeUnitpaint
	case "EventSubTypeDelete":
		*v = EventSubTypeDelete
	case "EventSubTypeAnother":
		*v = EventSubTypeAnother
	default:
		*v = EventSubTypeUnknown 
	}
	return nil
}

func cleanFloat(v float64)
