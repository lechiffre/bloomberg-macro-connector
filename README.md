# BlpConn: B-PIPE Connector

BlpConn is a C++/Go library for working with the Bloomberg Market Data Feed
(B-PIPE). At this moment, BlpConn focuses on retrieving data from Bloomberg's
economic data feed (//blp/economic-data).

## Subscription Request

A subscription request is a structure that encapsulates the required data to
activate a subscription. Once the subscription has been activated, your
application will start receiving updates and notifications about your request.

A request contains:

* `service`: One of the services offered by Bloomberg. At this moment, the only
  service enabled is `economic-data`.
* `subscription_type`: A category of information, such as a calendar of releases
  or economic headlines.
* `topic_type`: The standard used to encode the entity or security you are
  requesting information about. The most commonly used is "Ticker." Other types
  include CUSIP and FIGI.
* `topic`: The specific identifier that represents the entity or security
  you are requesting information about.
* `options`: Some requests may require complementary information, such as a period of time. This attribute is used to encode those additional parameters.
* `correlation_id`: A number to identify the request. This number can be assigned by the user or automatically assigned by the library.

For subscription and topic types, the library provides predefined enumerations
with valid values.

## Observer Functions

To receive notifications, the library requires you to register an observer
function. This function will be used as a callback: every time a new event
occurs, a notification will be delivered to the client program.

```c++
void observer(uint8_t *buffer, size_t len)
```

Notifications are delivered as [FlatBuffers](https://flatbuffers.dev/) binary
sequences. FlatBuffers is a high-performance serialization format and library. The client
program is responsible for implementing mechanisms to deserialize the data.
Examples of deserialization functions are provided.

## Usage Process

This is a basic example of using the library written in Go:

```go
ctx := blpconngo.NewContext()
ctx.AddNotificationHandler(Callback)
configPath := "./config.json"
res := ctx.InitializeSession(configPath)
if !res {
    log.Fatal("Failed to initialize service")
}
defer ctx.ShutdownService()
request := blpconngo.NewSubscriptionRequest()
request.SetTopic("CATBTOTB Index")
request.SetSubscription_type(
    blpconngo.SubscriptionType_ReleaseCalendar
)
ctx.Subscribe(request)
```

The process to use the library is described below:

1. Create a `Context` instance. This context keeps track of the session status
   and opened services.
2. Register an observer function.
3. Initialize the session. You must provide the path to the configuration file.
4. Prepare a subscription request.
5. Submit the request for subscription or unsubscription.
6. Shut down the service.

Steps 4 and 5 are supposed to be repeated many times while the session is
open. Asynchronously, your observer function will process session,
service, and subscription notifications.

## Configuration

The configuration file is intended to set the parameters and credentials needed
to use Bloomberg's services. A `config-example.json` file is included. Parameters to define are:

* `client_certificate`: Path to the file with the client certificate
* `root_certificate`: Path to the file with the root certificate
* `password`: Password to access Bloomberg's services
* `primary_host`: Name of Bloomberg's primary host
* `secondary_host`: Name of Bloomberg's secondary host
* `port`: Server port to access the services
* `default_service`: Default service identification
* `app_name`: Bloomberg's designated application name

## Requirements

To compile the library, the following are required:

- Blpapi SDK 3.25.3
- Boost 1.88
- Google Test 1.16
- Swig 4.3
- Go 1.24
- FlatBuffers 25.2

## FlatBuffers Schema

The FlatBuffers schema is located in the folder `fb`. It contains definitions for the following basic types:

* `ReleaseStatus`: Applicable to calendars to indicate if they have already been released or are just scheduled.
* `EventType`: Economic events can be actual, revisions, estimates, or calendars.
* `EventSubType`: Economic events can be new, update, unitpaint, or delete.
* `DateTime`: A generic time representation in epoch microseconds and an offset.
* `Value`: Represents a single value or a statistical distribution.

For a `Value` type, if the number of values is 1, then it is a single value
and can be accessed in the field `value`. Otherwise, you should access the
statistical attributes:

* `number`: Number of values
* `value`: When it is a single value
* `low`: Minimum value
* `high`: Maximum value
* `median`: The distribution median
* `average`: The mean
* `standard_deviation`: The standard deviation

The composite types are:

* `LogMessage`: A message related to session, service, or subscription status.
* `Heartbeat`: A signal that the session is working.
* `HeadlineEconomicEvent`: Represents the description and values of an
  economic indicator.
* `HeadlineCalendarEvent`: Time data about a scheduled or released economic
  event.

To generate headers and bindings for every language:

```sh
cd fb
make
```

The following files are generated:

* C++: `./include/blpconn_fb_generated.h`
* Go: `./go/BlpConn/FB`
* Python: `./vrs/python/BlpConn`

## Economic Events

All economic events share these fields:

* `id_bb_global`: A Bloomberg global ID
* `parsekyable_des`: Encoded description
* `description`: Human-readable description
* `event_type`
* `event_subtype`
* `event_id`: An identification number
* `observation_period`: The time frame the event corresponds to
* `release_start_dt`: Time when the data release starts
* `release_end_dt`: Time when the data release ends. If the event corresponds to a single moment, then start and end times are the same.

Headlines for economic events have these additional fields:

* `value`: The single value or the distribution of values
* `prior`: Data about a previous related event

Calendar events add one more field to basic economic events:

* `release_status`: Indicates if the event is scheduled or already
  released.
  
## Deserialization

Custom data structures and functions can be implemented to deserialize the notifications. The library includes two examples:

* For `C++`: `include/blpconn_message.h`
* For `Go`: `go/message.go`

As a high-level example, the next lines show an `observer` function that is
deserializing economic events and log messages:

```c++
void observer(const uint8_t *buffer, size_t size) {
    flatbuffers::Verifier verifier(buffer, size);
    if (!BlpConn::FB::VerifyMessageVector(verifier, nullptr, nullptr)) {
        std::cout << "Invalid message" << std::endl;
        return;
    }
    auto main = flatbuffers::GetRoot<BlpConn::FB::Main>(buffer);
    if (main->message_type() == BlpConn::FB::Message_HeadlineEconomicEvent) {
        auto fb_event = main->message_as_HeadlineEconomicEvent();
        auto event = toHeadlineEconomicEvent(fb_event);
        std::cout << event << std::endl;
    } else if (main->message_type() == FB::Message_HeadlineCalendarEvent) {
        auto fb_event = main->message_as_HeadlineCalendarEvent();
        auto event = toHeadlineCalendarEvent(fb_event);
        std::cout << event << std::endl;
    } else if (main->message_type() == BlpConn::FB::Message_LogMessage) {
        auto fb_log_message = main->message_as_LogMessage();
        auto log_message = toLogMessage(fb_log_message);
        std::cout << log_message << std::endl;
    }
}
```

Example functions for deserialization are included in:

* C++: `./include/blpconn_deserialize.h`
* Go: `./go/deserialize` (work in progress)

## Library Compilation

To generate the library, `cmake` is used.

```sh
mkdir build
cd build
cmake --build .
cmake --install .
```

The static library is located at `./lib/libblpconn.a`. Previous steps also compile example and test programs.

## SWIG C++/Go Wrappers

Wrappers from C++ to Go are generated using SWIG.

```sh
cd swig
make
```

The following files are generated:

* `./lib/libblpconngo.a`: A static library with C/C++ wrapper functions
* `./go/blpconngo`: A Go interface for BlpConn function calls.

## C++ Examples

Two C++ examples are provided:

* `simple`: A minimal example of connecting to Bloomberg B-PIPE and making a request.
* `cli`: A basic command line interface.

The `cli` example recognizes the following commands:

```text
subscribe SUBSCRIPTION_TYPE TOPIC
unsubscribe SUBSCRIPTION_TYPE TOPIC
```

This is a running example:

```
> subscribe economic CATBTOTB Index

HeadlineBaseEvent { id_bb_global: BBG002SBJ964, parsekyable_des: CATBTOTB
Index, description: STCA Canada Merchandise Trade Total Balance SA CAD,
event_type: REVISION, event_subtype: INITPAINT, event_id: 2167796,
observation_period: Feb, release_start_dt: { microseconds: 1743701400000000,
offset: 0 }, release_end_dt: { microseconds: 1743701400000000, offset: 0 } },
value: ValueType { number: 1, value: 3.13, low: nan, high: nan, median: nan,
average: nan, standard_deviation: nan }, prior_value: ValueType { number: 1,
value: 3.97, low: nan, high: nan, median: nan, average: nan,
standard_deviation: nan }, prior_event_id: 2167795, prior_observation_period:
Jan, prior_economic_release_start_dt: { microseconds: 0, offset: 0 },
prior_economic_release_end_dt: { microseconds: 0, offset: 0 } }
```

## Go Example

[This is work in progress]

A Go example is included. It is similar to the C++ cli program. A Go program requires:

* Libraries: `./lib/libblpconngo.a`, `./lib/libblpconn.a`, 
* Swig C++/Go wrappers: They are located in `./go/blpconngo`
* FlatBuffers: Binary serialization/deserialization interface

The observer and high-level deserialization function is shown below:

```go
//export NotificationHandler
func NotificationHandler(buffer *C.uchar, len C.size_t) {
    if buffer == nil || len == 0 {
        fmt.Println("Invalid buffer or length")
        return
    }
    bufferSlice := C.GoBytes(unsafe.Pointer(buffer), C.int(len))
    main := FB.GetRootAsMain(bufferSlice, 0)
    if main == nil {
        fmt.Println("Failed to parse FlatBuffers main object")
        return
    }
    switch main.MessageType() {
    case FB.MessageHeadlineEconomicEvent:
        var fbEvent = FB.GetRootAsHeadlineEconomicEvent(bufferSlice, 0)
        event := DeserializeHeadlineEconomicEvent(fbEvent)
        fmt.Println(event)
    case FB.MessageHeadlineCalendarEvent:
        var fbEvent = FB.GetRootAsHeadlineCalendarEvent(bufferSlice, 0)
        event := DeserializeHeadlineCalendarEvent(fbEvent)
        fmt.Println(event)
    case FB.MessageLogMessage:
        var fbLogMessage = FB.GetRootAsLogMessage(bufferSlice, 0)
        logMessage := DeserializeLogMessage(fbLogMessage)
        fmt.Println(logMessage)
    default:
        fmt.Println("Unknown message type")
    }
}
```

The comment above the observer function is needed in order to pass that function as a C object. That function is encapsulated in a C function:

```c
void Callback(uint8_t* buffer, size_t len) {
    NotificationHandler(buffer, len);
}
```

And the encapsulating function is passed in Go as a C pointer:

```go
...
var Callback = (*byte)(unsafe.Pointer(C.Callback))
...
ctx.AddNotificationHandler(Callback)
...
```

The Go program should have `cgo` parameters to compile the `blpconn` libraries. For example:

```go
/*
#cgo LDFLAGS: -L../lib -lblpapi3_64 -lblpconngo -lblpconn -lstdc++
#include <stdlib.h>
*/
import "C"
```

To build and run the Go example:

```sh
go build
./gocli
```

## TODO

1. Optimize the transfer from Bloomberg's messages to FlatBuffers binaries
  (a balance between performant code and maintainable code)
2. Fix issues and extend the Go example (issues on deserializing)
3. Extend test cases
4. Implement `HeartBeat` feature
5. Refine session, service, and subscription log messages
6. Add instrumentation to evaluate perfomance
7. Improve documentation
