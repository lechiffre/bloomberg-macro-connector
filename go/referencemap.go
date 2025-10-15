package blpconngo

import (
	"fmt"
)

type ReferenceMap struct {
	items map[uint64]MacroReferenceData
}

func NewReferenceMap() ReferenceMap {
	return ReferenceMap{
		items: make(map[uint64]MacroReferenceData),
	}
}

func (refMap ReferenceMap) Add(ref MacroReferenceData) {
	refMap.items[ref.CorrelationID] = ref
}

func (refMap ReferenceMap) Get(corrID uint64) (*MacroReferenceData, error) {
	ref, ok := refMap.items[corrID]
	if !ok {
		return nil, fmt.Errorf("No reference available for correlation ID %d", corrID)
	}
	return &ref, nil
}

func (refMap ReferenceMap) Remove(corrID uint64) {
	if _, ok := refMap.items[corrID]; ok {
		delete(refMap.items, corrID)
	}
}


// To check if a log message related to a subscription is 
// indicating that the subscription has ended. In that case,
// the reference for that subscription is removed in the map.
func (refMap ReferenceMap) LookAndRemove(event LogMessageType) {
	if event.Module == ModuleSubscription {
		status := SubscriptionStatus(event.Status)
		failure := (status == SubscriptionFailure)
		terminated := (status == SubscriptionTerminated)
		if failure || terminated {
			refMap.Remove(event.CorrelationID);
		}
	}
}

func (refMap ReferenceMap) fillHeadlineEvent(event MacroHeadlineEvent) HeadlineEvent {
	ref, ok := refMap.items[event.CorrelationID] 
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
	ref, ok := refMap.items[event.CorrelationID] 
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
