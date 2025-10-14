# Data for Testing

| File        | Type of Message  |
+-------------+------------------+
|fb_000001.bin|LogMessage        |
|fb_000002.bin|LogMessage        |
|fb_000003.bin|LogMessage        |
|fb_000004.bin|LogMessage        |
|fb_000005.bin|LogMessage        |
|fb_000006.bin|LogMessage        |
|fb_000007.bin|LogMessage        |
|fb_000008.bin|MacroReferenceData|
|fb_000009.bin|MacroCalendarEvent|
|fb_000010.bin|MacroCalendarEvent|
|fb_000011.bin|MacroCalendarEvent|
|fb_000012.bin|MacroCalendarEvent|
|fb_000013.bin|MacroHeadlineEvent|
|fb_000015.bin|LogMessage        |
|fb_000016.bin|LogMessage        |
|fb_000017.bin|LogMessage        |
|fb_000018.bin|LogMessage        |


## Content


>>> fb_000001.bin 
2025-10-10
18:36:20|Session|ConnectionUp|CorrelationID(0)|SessionConnectionUp = {
server = "gbr.cloudpoint.bloomberg.com:8194" serverId =
"ba44595-apicszfb" encryptionStatus = "Encrypted" encryptionCipher =
"TLS_AES_256_GCM_SHA384  TLSv1.3 Kx=any      Au=any  Enc=AESGCM(256)
Mac=AEAD" compressionStatus = "Compressed" }

>>> fb_000002.bin 
2025-10-10 18:36:20|Session|Started|CorrelationID(0)|SessionStarted = {
initialEndpoints[] = { initialEndpoints = { address =
"gbr.cloudpoint.bloomberg.com:8194" } } }

>>> fb_000003.bin 
2025-10-10
18:36:20|Session|ConnectionUp|CorrelationID(0)|SessionConnectionUp = {
server = "deu.cloudpoint.bloomberg.com:8194" serverId =
"ba39334-apicszfb" encryptionStatus = "Encrypted" encryptionCipher =
"TLS_AES_256_GCM_SHA384  TLSv1.3 Kx=any      Au=any  Enc=AESGCM(256)
Mac=AEAD" compressionStatus = "Compressed" }

>>> fb_000004.bin 
2025-10-10 18:36:20|Service|Opened|CorrelationID(0)|ServiceOpened = {
serviceName = "//blp/macro-indicators" }

>>> fb_000005.bin 
2025-10-10 18:36:20|Subscription|Success|CorrelationID(12)|Subscription
successful

>>> fb_000006.bin 
2025-10-10
18:36:21|Subscription|Started|CorrelationID(12)|SubscriptionStarted = {
exceptions[] = { } streamIds[] = { "1" } receivedFrom = { address =
"gbr.cloudpoint.bloomberg.com:8194" } reason = "Subscriber made a
subscription" }

>>> fb_000007.bin 
2025-10-10
18:36:21|Subscription|StreamsActivated|CorrelationID(12)|SubscriptionStreamsActivated
= { streams[] = { streams = { id = "1" endpoint = { address =
"gbr.cloudpoint.bloomberg.com:8194" serverId = "ba44595-apicszfb" } } }
reason = "Subscriber made a subscription" }

>>> fb_000008.bin 
MacroReferenceData { corr_id: 12, id_bb_global: BBG002SBJ964,
parsekyable_des: CATBTOTB Index, description: STCA Canada Merchandise
Trade Total Balance SA CAD, indx_freq: Monthly, indx_units: Value,
country_iso: CA, indx_source: STCA - Statistics Canada,
seasonality_transformation: Value SA }

>>> fb_000009.bin 
MacroCalendarEvent { corr_id: 12, id_bb_global: , parsekyable_des: ,
event_type: CALENDAR, event_subtype: INITPAINT, description: , event_id:
2167803, observation_period: Sep, release_start_dt: { microseconds:
1762263000000000, offset: 0 }, release_end_dt: { microseconds:
1762263000000000, offset: 0 }, release_status: SCHEDULED,
relevance_value: 55.2632 }

>>> fb_000010.bin 
MacroCalendarEvent { corr_id: 12, id_bb_global: , parsekyable_des: ,
event_type: CALENDAR, event_subtype: INITPAINT, description: , event_id:
2167804, observation_period: Oct, release_start_dt: { microseconds:
1764855000000000, offset: 0 }, release_end_dt: { microseconds:
1764855000000000, offset: 0 }, release_status: SCHEDULED,
relevance_value: 55.2632 }

>>> fb_000011.bin 
MacroCalendarEvent { corr_id: 12, id_bb_global: , parsekyable_des: ,
event_type: CALENDAR, event_subtype: INITPAINT, description: , event_id:
2167805, observation_period: Nov, release_start_dt: { microseconds:
1767879000000000, offset: 0 }, release_end_dt: { microseconds:
1767879000000000, offset: 0 }, release_status: SCHEDULED,
relevance_value: 55.2632 }

>>> fb_000012.bin 
MacroCalendarEvent { corr_id: 12, id_bb_global: , parsekyable_des: ,
event_type: CALENDAR, event_subtype: INITPAINT, description: , event_id:
2167806, observation_period: Dec, release_start_dt: { microseconds:
1770298200000000, offset: 0 }, release_end_dt: { microseconds:
1770298200000000, offset: 0 }, release_status: SCHEDULED,
relevance_value: 55.2632 }

>>> fb_000013.bin 
MacroHeadlineEvent { corr_id: 12, event_type: ACTUAL, event_subtype:
INITPAINT, event_id: 2167802, observation_period: Aug, release_start_dt:
{ microseconds: 1759840200000000, offset: 0 }, release_end_dt: {
microseconds: 1759840200000000, offset: 0 }, prior_event_id: 0,
prior_observation_period: , prior_economic_release_start_dt: {
microseconds: 18444534998509551616, offset: 0 },
prior_economic_release_end_dt: { microseconds: 18444534998509551616,
offset: 0 }, value: ValueType { number: 1, value: -6.32, low: nan, high:
nan, median: nan, average: nan, standard_deviation: nan } }

>>> fb_000014.bin 
MacroHeadlineEvent { corr_id: 12, event_type: REVISION, event_subtype:
INITPAINT, event_id: 2167802, observation_period: Aug, release_start_dt:
{ microseconds: 1759840200000000, offset: 0 }, release_end_dt: {
microseconds: 1759840200000000, offset: 0 }, prior_event_id: 2167801,
prior_observation_period: Jul, prior_economic_release_start_dt: {
microseconds: 1756989000000000, offset: 0 },
prior_economic_release_end_dt: { microseconds: 1756989000000000, offset:
0 }, value: ValueType { number: 1, value: -3.82, low: nan, high: nan,
median: nan, average: nan, standard_deviation: nan } }

>>> fb_000015.bin 
2025-10-10
18:36:30|Subscription|Terminated|CorrelationID(12)|SubscriptionTerminated
= { reason = { source = "SubscriptionManager" category = "CANCELLED"
errorCode = 0 description = "Subscription cancelled" } }

>>> fb_000016.bin 
2025-10-10
18:36:30|Session|ConnectionDown|CorrelationID(0)|SessionConnectionDown =
{ server = "gbr.cloudpoint.bloomberg.com:8194" serverId =
"ba44595-apicszfb" }

>>> fb_000017.bin 
2025-10-10
18:36:30|Session|ConnectionDown|CorrelationID(0)|SessionConnectionDown =
{ server = "deu.cloudpoint.bloomberg.com:8194" serverId =
"ba39334-apicszfb" }

>>> fb_000018.bin 
2025-10-10
18:36:30|Session|Terminated|CorrelationID(0)|SessionTerminated = { }
