# Bloomberg GECO Connector

headline-actuals, release-calendar, headline-surveys"

EconomicEvent = { HeadlineEconomicEvent = { ID_BB_GLOBAL = "BBG002SBJ964" PARSEKYABLE_DES = "CATBTOTB Index" DESCRIPTION = "STCA Canada Merchandise Trade Total Balance SA CAD" EVENT_TYPE = REVISION EVENT_SUBTYPE = INITPAINT EVENT_ID = 2167796 OBSERVATION_PERIOD = "Feb" ECO_RELEASE_DT = { DATETIME = 2025-04-03T12:30:00.000+00:00 } VALUE = { SINGLE = "3.130" } PRIOR_VALUE = { SINGLE = "3.970" } REVISION_METADATA = { PRIOR_EVENT_ID = 2167795 PRIOR_OBSERVATION_PERIOD = "Jan" PRIOR_ECO_RELEASE_DT = { DATETIME = 2025-03-06T13:30:00.000+00:00 } } } }


    return true;
SessionConnectionUp = { server = "gbr.cloudpoint.bloomberg.com:8194" serverId = "ba40046-apicszfb" encryptionStatus = "Encrypted" encryptionCipher = "TLS_AES_256_GCM_SHA384  TLSv1.3 Kx=any      Au=any  Enc=AESGCM(256) Mac=AEAD" compressionStatus = "Compressed" }

SessionStarted = { initialEndpoints[] = { initialEndpoints = { address = "gbr.cloudpoint.bloomberg.com:8194" } } }

SessionConnectionUp = { server = "deu.cloudpoint.bloomberg.com:8194" serverId = "ba41068-apicszfb" encryptionStatus = "Encrypted" encryptionCipher = "TLS_AES_256_GCM_SHA384  TLSv1.3 Kx=any      Au=any  Enc=AESGCM(256) Mac=AEAD" compressionStatus = "Compressed" }

ServiceOpened = { serviceName = "//blp/economic-data" }

SubscriptionStarted = { exceptions[] = { } streamIds[] = { "1" } receivedFrom = { address = "gbr.cloudpoint.bloomberg.com:8194" } reason = "Subscriber made a subscription" }

SubscriptionStreamsActivated = { streams[] = { streams = { id = "1" endpoint = { address = "gbr.cloudpoint.bloomberg.com:8194" serverId = "ba40046-apicszfb" } } } reason = "Subscriber made a subscription" }

Heartbeat

SessionConnectionDown = { server = "gbr.cloudpoint.bloomberg.com:8194" serverId = "ba40046-apicszfb" }

SessionTerminated = { }

