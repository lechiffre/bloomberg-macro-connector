# BlpConn: B-PIPE Connector

BlpConn is a C++/Go library to work with the Bloomberg Market Data Feed
(B-PIPE).  At this moment, BlpConn is focused on retrieving data form the
Bloomberg's economic data feed (//blp/economic-data).

## Subscription Request

A subscription request is a structure that encapsules the required data to
activate a subscription. Once the subscription has been activated, your
application will start receiving updates and notifications about your request.

A request contains:

* `service`: One of the services offered by Bloombergs. At this moment, the only
  service enabled is `economic-data`.
* `subscription_type`: It is a category of information, like calendar of releases
  or economic headlines.
* `topic_type`: It is the standard to encode the entity or security you are
  requiring information. The most used is "Ticker". Other types include CUSIP
  and FIGI.
* `topic`: This is the specefic identifier that represents the entity or security
  you are requiring information.
* `options`: Some requests may need complementary information, for example a period of time. This attribute has the purpose to encode those additional parameters.
* `correlation_id`: A number to idetinfy the request. This number can be assigned by the user or automacatically assigned by the library.

For subscription and topic types, the library provides predefined enumerations
with valid values.

## Observer functions

To receive notifications, the library requires that you register a observer function.
This function will be used as a callback: every time a new event occurs, a notification will be deliverated to the client program.

Notifications are delivered as [FlatBuffers](https://flatbuffers.dev/) binary
sequences. FlatBuffers is a high performant serialization format and library. The client
program is responsible to implement mechanisms to deserialize the data. Examples of deserialization functions are provided for C++ and Go.

## Usage process


