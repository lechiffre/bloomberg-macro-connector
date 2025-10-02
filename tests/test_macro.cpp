/*

MacroEvent = { MacroReferenceData = { ID_BB_GLOBAL = "BBG002SBJ964" PARSEKYABLE_DES = "C
ATBTOTB Index" DESCRIPTION = "STCA Canada Merchandise Trade Total Balance SA CAD" INDX_F
REQ = "Monthly" INDX_UNITS = "Value" COUNTRY_ISO = "CA" INDX_SOURCE = "STCA - Statistics Canada" SEASONALITY_AND_TRANSFORMATION = "Value SA" } }                                2025-10-01 20:52:15|Heartbeat|Unknown|CorrelationID(1)|Subscription Heartbeat
MacroEvent = { MacroCalendarEvent = { EVENT_TYPE = CALENDAR EVENT_SUBTYPE = INITPAINT OBSERVATION_PERIOD = "Aug" ECO_RELEASE_DT = { DATETIME = 2025-10-07T12:30:00.000+00:00 } E
VENT_ID = 2167802 RELEASE_STATUS = SCHEDULED RELEVANCE_VALUE = 55.263200 } } 

MacroEvent = { MacroCalendarEvent = { EVENT_TYPE = CALENDAR EVENT_SUBTYPE = INITPAINT OB
SERVATION_PERIOD = "Sep" ECO_RELEASE_DT = { DATETIME = 2025-11-04T13:30:00.000+00:00 } E
VENT_ID = 2167803 RELEASE_STATUS = SCHEDULED RELEVANCE_VALUE = 55.263200 } }

MacroEvent = { MacroCalendarEvent = { EVENT_TYPE = CALENDAR EVENT_SUBTYPE = INITPAINT OB
SERVATION_PERIOD = "Oct" ECO_RELEASE_DT = { DATETIME = 2025-12-04T13:30:00.000+00:00 } E
VENT_ID = 2167804 RELEASE_STATUS = SCHEDULED RELEVANCE_VALUE = 55.263200 } }

MacroEvent = { MacroCalendarEvent = { EVENT_TYPE = CALENDAR EVENT_SUBTYP20:52:24 [2/434]
SERVATION_PERIOD = "Oct" ECO_RELEASE_DT = { DATETIME = 2025-12-04T13:30:00.000+00:00 } E
VENT_ID = 2167804 RELEASE_STATUS = SCHEDULED RELEVANCE_VALUE = 55.263200 } }

MacroEvent = { MacroCalendarEvent = { EVENT_TYPE = CALENDAR EVENT_SUBTYPE = INITPAINT OB
SERVATION_PERIOD = "Nov" ECO_RELEASE_DT = { DATETIME = 2026-01-08T13:30:00.000+00:00 } E
VENT_ID = 2167805 RELEASE_STATUS = SCHEDULED RELEVANCE_VALUE = 55.263200 } }

MacroEvent = { MacroCalendarEvent = { EVENT_TYPE = CALENDAR EVENT_SUBTYPE = INITPAINT OB
SERVATION_PERIOD = "Dec" ECO_RELEASE_DT = { DATETIME = 2026-02-05T13:30:00.000+00:00 } E
VENT_ID = 2167806 RELEASE_STATUS = SCHEDULED RELEVANCE_VALUE = 55.263200 } }

MacroEvent = { MacroHeadlineEvent = { EVENT_TYPE = ACTUAL EVENT_SUBTYPE = INITPAINT EVEN
T_ID = 2167801 RELEVANCE_VALUE = 55.263200 OBSERVATION_PERIOD = "Jul" ECO_RELEASE_DT = {
 DATETIME = 2025-09-04T12:30:00.000+00:00 } VALUE = { SINGLE = -4.940000 } } }

MacroEvent = { MacroHeadlineEvent = { EVENT_TYPE = REVISION EVENT_SUBTYPE = INITPAINT EV
ENT_ID = 2167801 RELEVANCE_VALUE = 55.263200 OBSERVATION_PERIOD = "Jul" ECO_RELEASE_DT =
 { DATETIME = 2025-09-04T12:30:00.000+00:00 } VALUE = { SINGLE = -5.980000 } PRIOR_VALUE
 = -5.860000 REVISION_METADATA = { PRIOR_EVENT_ID = 2167800 PRIOR_OBSERVATION_PERIOD = "
Jun" PRIOR_ECO_RELEASE_DT = { DATETIME = 2025-08-05T12:30:00.000+00:00 } } } }

*/

#include <blpconn.h>
#include <gtest/gtest.h>
#include <thread>
#include <chrono>

using namespace BlpConn;

TEST(Context, Subscription) {
    Context ctx;
    std::string config_path = "./config.json";
    EXPECT_TRUE(ctx.initializeSession(config_path));
    SubscriptionRequest request = {
            .topic = "CATBTOTB Index",
            .correlation_id = 1
    };
    EXPECT_EQ(ctx.subscribe(request), 0);
    std::this_thread::sleep_for(std::chrono::seconds(10));
    ctx.unsubscribe(request);
    ctx.shutdownSession();
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
