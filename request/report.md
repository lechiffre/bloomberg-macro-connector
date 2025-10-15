### Report: Bloomberg Data Connector Update

Oct. 14, 2025

This report details the implementation of changes to the Bloomberg data
connector to integrate the `//blp/macro-indicators` service.

The C++ library was updated to process new message formats and
subscription strings while continuing its role as a stateless data
conduit. An architectural change was introduced in the Go implementation
with a feature called the "Managed Context". This layer introduces a
stateful cache, referred to as a `reference map`, which uses a
`correlation_id` to link an initial `MacroReferenceData` record with
subsequent `MacroHeadlineEvent` and `MacroCalendarEvent` messages. This
process results in the delivery of enriched data objects to the end-user
application.

For the Go implementation, enumerations were defined with their own data
types, changing from their previous implementation as integers. The
Managed Context also provides subscription functions, such as
`subscribeTicker`, and automates the cancellation of active
subscriptions when a session is terminated. Bindings for C++,  and Go
were regenerated from an updated FlatBuffers schema, and testing was
executed to validate serialization, connectivity, and the subscription
logic.

#### **C++ Implementation**

* **Configuration Updates:** The `default_service` in configuration
  files, including `config-example.json`, was updated to
  `//blp/macro-indicators`.
* **Subscription String Construction:** The logic for creating
  subscription strings was modified, removing the `subscription_type`
  concept to use the new topic format.
* **FlatBuffers Schema Updates:** The `fb/blpconn_fb.fbs` schema file
  was updated with definitions for new message types:
  `MacroReferenceData`, `MacroHeadlineEvent`, and `MacroCalendarEvent`.
* **Correlation ID Propagation:** A `correlation_id` field was added to
  all new message types within the schema to link reference data with
  their corresponding events. The C++ source code was updated to handle
  this ID during event processing.
* **FlatBuffers Bindings Generation:** Language-specific bindings for
  C++ and Go were regenerated from the updated schema to incorporate the
  new data types and fields.

#### **Go Implementation**

* **New Data Structures:** "Extended" data structures were defined in Go
  to combine the fields from event messages with the corresponding
  cached reference data.
* **State Management:** A `reference map` was implemented to function as
  an in-memory cache. It stores `MacroReferenceData`, using the
  `correlation_id` to enrich incoming headline and calendar events.
* **Subscription Workflow Updates:** The "Managed Context" was
  introduced to handle the subscription process via a single function
  call and to automate unsubscription from topics upon session shutdown.
* **NotificationHandler Callback Re-architecture:** A default
  notification handler was created to demonstrate the new workflow. This
  handler decodes messages, uses the `reference map` to enrich event
  data, and removes entries from the cache upon receiving a subscription
  failure notification.
* **Client-Side Event Filtering:** An `EventType` enumeration was
  defined in the FlatBuffers schema, making it available in the Go
  bindings for client-side event filtering.
