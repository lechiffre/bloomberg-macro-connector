#!/usr/bin/env -S uv run --script
# /// script
# dependencies = ["blpapi", "python-dotenv"]
# ///

import os
import blpapi
import math
import time
from dotenv import load_dotenv
load_dotenv()

def _parseEconomicEvent(elem: blpapi.Element):
    builder = flatbuffers.Builder()
    if elem.hasElement("HeadlineEconomicEvent"):
        sub_elem = elem.getElement("HeadlineEconomicEvent")
        print(sub_elem)
        id_bb_global = builder.CreateString(sub_elem.getElement("ID_BB_GLOBAL").getValue(0))
        parsekyable_des = builder.CreateString(sub_elem.getElement("PARSEKYABLE_DES").getValue(0))
        description = (
            ""
            if not sub_elem.hasElement("DESCRIPTION")
            else builder.CreateString(sub_elem.getElement("DESCRIPTION").getValue(0))
        )
        eventype = (
            ""
            if not sub_elem.hasElement("EVENTYPE")
            else builder.Create
            
            (sub_elem.getElement("EVENTYPE").getValue(0))
        )
    return builder

"""
namespace BlpConn.Message;

table DateTime {
    millis: ulong;
    offset: short;
}

table DateRange {
    start: DateTime;
    end: DateTime;
}

union ReleaseTime { DateTime, DateRange }

table Distribution {
    low: float;
    high: float;
    median: float;
    average: float;
    standard_deviation: float;
    number: float;
}

table Single {
    number: float;
}

union Value { Single, Distribution }

table RevisionMetadata {
    prior_event_id: int;
    prior_observation_period: string;
    prior_economic_release: ReleaseTime;
}

table HeadlineEconomicEvent {
    id_bb_global: string (required);
    parsekyable_des: string (required);
    description: string;
    event_type: string;
    event_subtype: string;
    event_id: int;
    observation_period: string;
    eco_release_dt: ReleaseTime (required);
    value: Value;
    prior_value: Value;
    revision_metadata: RevisionMetadata;
}

table HeadlineCalendarEvent {
    id_bb_global: string (required);
    parsekyable_des: string (required);
    description: string;
    event_type: string;
    event_subtype: string;
    event_id: int;
    observation_period: string;
    eco_release_dt: ReleaseTime (required);
    release_status: string;
}

table EmptyEconomicEvent {}

table HeartBeat {
}

union EconomicEvent { HeadlineEconomicEvent, HeadlineCalendarEvent, EmptyEconomicEvent }

table DateTime {
    year: ushort = 0;
    month: ubyte = 0;
    day: ubyte = 0;
    hours: ubyte = 0;
    minutes: ubyte = 0;
    seconds: ubyte = 0;
    micros: ushort = 0;
    offset: short = 0;
    parts: uint = 0;
}
""""

def getSringValue(elem: blpapi.Element, name: str):
    if elem.hasElement(name):
        return elem.getElement(name).getValue(0)
    return ""


def getDateTimeValue(elem: blpapi.Element):
    return {
        "year": elem.year(),
        "month": elem.month(),
        "day": elem.day(),
        "hours": elem.hours(),
        "minutes": elem.minutes(),
        "seconds": elem.seconds(),
        "micros": elem.micros(),
        "offset": elem.offset(),
        "parts": elem.parts(),
    }

def getReleaseTime(elem: blpapi.Element, name: str):
    if not elem.hasElement(name):
        return None
    sub_elem = elem.getElement(name)
    if sub_elem.hasElement("DATETIME") or sub_elem.hasElement("DATE"):
        return {
            "single": getDateTimeValue(elem.getElement("DATETIME")),
        }
    elif sub_elem.hasElement("DATERANGE"):
        return {
            "start": getDateTimeValue(elem.getElement("DATERANGE").getElement("START")),
            "end": getDateTimeValue(elem.getElement("DATERANGE").getElement("END")),
        }
    return None

def getFloatValue(elem: blpapi.Element, name: str):
    if not elem.hasElement(name):
        return math.nan
    return elem.getElement(name).getValue(0)

def getSingleValue(elem: blpapi.Element):
    return {
        "single": getFloatValue(elem, "SINGLE"),
    }

def getDistributionValue(elem: blpapi.Element):
    return {
        "low": getFloatValue(elem, "LOW"),
        "high": getFloatValue(elem, "HIGH"),
        "median": getFloatValue(elem, "MEDIAN"),
        "average": getFloatValue(elem, "AVERAGE"),
        "standard_deviation": getFloatValue(elem, "STANDARD_DEVIATION"),
        "number": getFloatValue(elem, "NUMBER"),
    }

def getValue(elem: blpapi.Element, name: str):
    if not elem.hasElement(name):
        return None
    sub_elem = elem.getElement(name)
    if sub_elem.hasElement("SINGLE"):
        return getSingleValue(sub_elem.getElement("SINGLE"))
    elif sub_elem.hasElement("DISTRIBUTION"):
        return getDistributionValue(sub_elem.getElement("DISTRIBUTION"))
    return None

def getRevisionMetadata(elem: blpapi.Element, name: str):
    if not elem.hasElement(name):
        return None
    sub_elem = elem.getElement(name)
    return {
        "prior_event_id": sub_elem.getElement("PRIOR_EVENT_ID").getValue(0),
        "prior_observation_period": getSringValue(sub_elem, "PRIOR_OBSERVATION_PERIOD"),
        "prior_economic_release": getReleaseTime(sub_elem, "PRIOR_ECONOMIC_RELEASE"),
    }

def parseHeadlineEconomicEvent(elem: blpapi.Element):
    return {
        "id_bb_global": getSringValue(elem, "ID_BB_GLOBAL"),
        "parsekyable_des": getSringValue(elem, "PARSEKYABLE_DES"),
        "description": getSringValue(elem, "DESCRIPTION"),
        "event_type": getSringValue(elem, "EVENTYPE"),
        "event_subtype": getSringValue(elem, "EVENT_SUBTYPE"),
        "event_id": elem.getElement("EVENT_ID").getValue(0),
        "observation_period": getSringValue(elem, "OBSERVATION_PERIOD"),
        "eco_release_dt": getReleaseTime(elem.getElement("ECO_RELEASE_DT")),
        "value": getValue(elem, "VALUE"),
        "prior_value": getValue(elem, "PRIOR_VALUE"),
        "revision_metadata": getRevisionMetadata(elem, "REVISION_METADATA"),
    }

def parseHeadlineCalendarEvent(elem: blpapi.Element):
    return {
        "id_bb_global": getSringValue(elem, "ID_BB_GLOBAL"),
        "parsekyable_des": getSringValue(elem, "PARSEKYABLE_DES"),
        "description": getSringValue(elem, "DESCRIPTION"),
        "event_type": getSringValue(elem, "EVENTYPE"),
        "event_subtype": getSringValue(elem, "EVENT_SUBTYPE"),
        "event_id": elem.getElement("EVENT_ID").getValue(0),
        "observation_period": getSringValue(elem, "OBSERVATION_PERIOD"),
        "eco_release_dt": getReleaseTime(elem.getElement("ECO_RELEASE_DT")),
        "release_status": getSringValue(elem, "RELEASE_STATUS"),
    } 

def parse_economic_event(elem: blpapi.Element):
    if elem.hasElement("HeadlineEconomicEvent"):
        return parseHeadlineEconomicEvent(elem.getElement("HeadlineEconomicEvent"))
    elif elem.hasElement("HeadlineCalendarEvent"):
        return parseHeadlineCalendarEvent(elem.getElement("HeadlineCalendarEvent"))
    return None

def connect_to_bpipe():
    sessionOptions = blpapi.SessionOptions()
    host_primary = os.environ["PRIMARY_HOST"]
    host_secondary = os.environ["SECONDARY_HOST"]
    port = int(os.environ["PORT"])
    sessionOptions.setServerAddress(host_primary, port, 0)
    sessionOptions.setServerAddress(host_secondary, port, 0)
    client_cert_path = os.environ["CLIENT_CERTIFICATE"]
    client_cert_password = os.environ["PASSWORD"]
    root_cert_path = os.environ["ROOT_CERTIFICATE"]
    tlsOptions = blpapi.TlsOptions.createFromFiles(
        client_cert_path,
        client_cert_password,
        root_cert_path
    )
    sessionOptions.setTlsOptions(tlsOptions)
    sessionOptions.setAutoRestartOnDisconnection(True)
    sessionOptions.setNumStartAttempts(3)
    session = blpapi.Session(sessionOptions)
    if not session.start():
        print("Failed to start session")
        return None
    print("Successfully connected to B-PIPE")
    return session

def geco_service(session):
    identify = session.createIdentity()
    session.openService("//blp/globaleco")
    service = session.getService("//blp/globalecoref")
    request = service.createRequest("GetTickersRequest")
    session.sendRequest(request, identity)
    

if __name__ == "__main__":
    session = connect_to_bpipe()
    geco_service(session)
    
