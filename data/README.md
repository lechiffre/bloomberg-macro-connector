# Data Directory

This directory contains binary files in FlatBuffers format captured from Bloomberg API interactions. These files are used for testing and validation purposes.

## File Inventory

The following binary files are stored in this directory:

### Log Messages (11 files)
- **fb_000001.bin**: SessionConnectionUp - Connection to gbr.cloudpoint.bloomberg.com:8194
- **fb_000002.bin**: SessionStarted - Initial endpoints configuration
- **fb_000003.bin**: SessionConnectionUp - Connection to deu.cloudpoint.bloomberg.com:8194
- **fb_000004.bin**: ServiceOpened - Opening //blp/macro-indicators service
- **fb_000005.bin**: Subscription Success - Subscription acknowledgment for CATBTOTB Index
- **fb_000006.bin**: SubscriptionStarted - Subscription stream started
- **fb_000007.bin**: SubscriptionStreamsActivated - Stream activation notification
- **fb_000014.bin**: Heartbeat - Subscription heartbeat message
- **fb_000015.bin**: SubscriptionTerminated - Subscription cancellation
- **fb_000016.bin**: SessionConnectionDown - Connection closed (gbr server)
- **fb_000017.bin**: SessionConnectionDown - Connection closed (deu server)
- **fb_000018.bin**: SessionTerminated - Session termination

### Macro Reference Data (1 file)
- **fb_000008.bin**: MacroReferenceData for CATBTOTB Index
  - ID: BBG002SBJ964
  - Description: STCA Canada Merchandise Trade Total Balance SA CAD
  - Frequency: Monthly
  - Country: CA

### Macro Calendar Events (3 files)
- **fb_000009.bin**: MacroCalendarEvent - October release (Event ID: 2167804)
- **fb_000010.bin**: MacroCalendarEvent - November release (Event ID: 2167805)
- **fb_000011.bin**: MacroCalendarEvent - December release (Event ID: 2167806)

### Macro Headline Events (2 files)
- **fb_000012.bin**: MacroHeadlineEvent - ACTUAL event
  - Event ID: 2167802
  - Observation Period: August
  - Value: -6.32 CAD
  - Release Date: 2025-10-07 12:30:00 UTC

- **fb_000013.bin**: MacroHeadlineEvent - REVISION event
  - Event ID: 2167802
  - Observation Period: August
  - Value: -3.82 CAD (revised from previous value)
  - Prior Event ID: 2167801 (July data)
  - Release Date: 2025-10-07 12:30:00 UTC

## Session Context

These files were captured during a subscription session to CATBTOTB Index (Canada Merchandise Trade Total Balance) on November 11, 2025. The session demonstrates the complete lifecycle of a Bloomberg API subscription:

1. **Connection establishment** to Bloomberg servers
2. **Service initialization** for macro indicators
3. **Subscription setup** for the specific security
4. **Data reception** including reference data, calendar events, and headline events
5. **Heartbeat** to maintain connection
6. **Subscription and session teardown**

## File Format

All files are serialized using FlatBuffers with the schema defined in the `fb/` directory. The main message types are:
- `LogMessage`: System and subscription lifecycle messages
- `MacroReferenceData`: Reference information about macro indicators
- `MacroCalendarEvent`: Scheduled future releases
- `MacroHeadlineEvent`: Actual data releases and revisions

## Usage

These files are primarily used by the test suite in `tests/test_loadfbbin.cpp` to verify:
- Correct deserialization of FlatBuffers data
- Proper handling of all message types
- Data integrity and validation

To view the raw log of the capture session, see `log.txt` in this directory.
