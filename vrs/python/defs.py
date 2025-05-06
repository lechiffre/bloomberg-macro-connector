from dataclasses import dataclass
from typing import Optional
import math

@dataclass
class DateTime:
    micros: int        # corresponds to FlatBuffers 'ulong'
    offset: int        # corresponds to FlatBuffers 'short'

@dataclass
class Value:
    number: int = 0
    value: float = math.nan
    low: float = math.nan
    high: float = math.nan
    median: float = math.nan
    average: float = math.nan
    standard_deviation: float = math.nan

@dataclass
class HeadlineEconomicEvent:
    id_bb_global: str
    parsekyable_des: str
    description: Optional[str] = None
    event_type: Optional[str] = None
    event_subtype: Optional[str] = None
    event_id: Optional[int] = None
    observation_period: Optional[str] = None
    release_start_dt: Optional[DateTime] = None
    release_end_dt: Optional[DateTime] = None
    value: Optional[Value] = None
    prior_value: Optional[Value] = None
    prior_event_id: Optional[int] = None
    prior_observation_period: Optional[str] = None
    prior_economic_release_start_dt: Optional[DateTime] = None
    prior_economic_release_end_dt: Optional[DateTime] = None

@dataclass
class HeadlineCalendarEvent:
    id_bb_global: str
    parsekyable_des: str
    description: Optional[str] = None
    event_type: Optional[str] = None
    event_subtype: Optional[str] = None
    event_id: Optional[int] = None
    observation_period: Optional[str] = None
    release_start_dt: Optional[DateTime] = None
    release_end_dt: Optional[DateTime] = None
    release_status: Optional[str] = None
    