package blpconngo

import (
	"sync"
	"fmt"
)

// Internal helper function to complement the label of
// the instrument with the type. The concatenated string
// is used as key in the subscriptions map.
func (topicType BlpConnTopicType) getLabel() string {
	if topicType == TopicType_Ticker {
		return "Ticker:"
	} else {
		return "Bbgid:"
	}
}

// ManagedContext simplifies the session management. It keeps
// reference of the subscription requested. Automatically manages
// the assignation of correlations id. The correlation id is used
// by the handler to merge the headline and calendar evets with the
// reference data. This extended context also automatically cancel
// the active subscription when the context is shutdown.
type ManagedContext struct {
	Context
	subscriptions map[string]SubscriptionRequest
	nextCorrId	  uint64
	mu			  sync.Mutex
}

// Returns a new context management. Once it is create, one or
// more notification handlers can be attached. The caller is
// responsible to call the ManagedShutdown the close the connection.
func NewManagedContext() ManagedContext {
	return ManagedContext{
		Context: NewContext(),
		subscriptions: make(map[string]SubscriptionRequest),
		nextCorrId: 0,
	}
}

// Automatically cancel the active subscriptions and close the
// connection to the Bloomberg server.
func (ctx *ManagedContext) ManagedShutdown() {
	for _, subs := range ctx.subscriptions {
		ctx.CancelSubscription(subs.GetTopic_type(), subs.GetTopic());
	}
	ctx.ShutdownSession()
}

// This the general method provide by the managed context to make
// subscriptions. However, specializated functions are provided for
// Tickers and Bbgid type topics.
func (ctx *ManagedContext) CreateSubscription(topicType BlpConnTopicType, instrument string) (uint64, error) {
	ctx.mu.Lock()
	defer ctx.mu.Unlock()
	// The key for the subscriptions map is formed by the
	// topic type and the instrument/topic
	key := topicType.getLabel() + instrument
	if _, exists := ctx.subscriptions[key]; exists {
		return 0, fmt.Errorf("%s already subscribed", instrument)
	}
	ctx.nextCorrId += 1
	// Creating a new request
	request := NewSubscriptionRequest()
	request.SetTopic(instrument)
	request.SetTopic_type(topicType)
	request.SetCorrelation_id(ctx.nextCorrId)
	ctx.Subscribe(request)
	// Updating the subscriptions map
	ctx.subscriptions[key] = request
	return ctx.nextCorrId, nil
}

// Removes a subscription from the map. This function don't cancel the
// subscription. Only removes it from memory. It should be called when the
// client receives a notification that the subscription has been ended
// for any reason different to an unsubscription request.
func (ctx *ManagedContext) RemoveSubscription(topicType BlpConnTopicType, instrument string) error {
 	ctx.mu.Lock()
 	defer ctx.mu.Unlock()
	key := topicType.getLabel() + instrument
	if _, ok := ctx.subscriptions[key]; !ok {
		return fmt.Errorf("%s is not subscribed", instrument)
	}
	delete(ctx.subscriptions, key)
	return nil
}

// Send an unsubscription request to the Bloomberg server and remove
// subscription from the map.
func (ctx *ManagedContext) CancelSubscription(topicType BlpConnTopicType, instrument string) error {
 	ctx.mu.Lock()
 	defer ctx.mu.Unlock()
	key := topicType.getLabel() + instrument
	subs, ok := ctx.subscriptions[key]
	if !ok {
		return fmt.Errorf("%s is not subscribed", instrument)
	}
	ctx.Unsubscribe(subs)
	delete(ctx.subscriptions, key)
	return nil
}

func (ctx ManagedContext) GetCorrelationId(topicType BlpConnTopicType, instrument string) (uint64, error) {
	key := topicType.getLabel() + instrument
	subs, ok := ctx.subscriptions[key];
	if !ok {
		return 0, fmt.Errorf("%s is not subscribed", instrument)
	}
	return subs.GetCorrelation_id(), nil
}

// Returns the list of subscriptions for the given topic type.
func (ctx ManagedContext) GetSubscribedTopics(topicType BlpConnTopicType) []string {
	keys := make([]string, 0)
	for _, subs := range ctx.subscriptions {
		if subs.GetTopic_type() == topicType {
			keys = append(keys, subs.GetTopic())
		}
	}
	return keys
}

// Specializated subscription function for tickers
func (ctx ManagedContext) SubscribeTicker(ticker string) (uint64, error) {
	return ctx.CreateSubscription(TopicType_Ticker, ticker)
}

// Specializated subscription function for Bbgids
func (ctx ManagedContext) SubscribeBbgid(bbgid string) (uint64, error) {
	return ctx.CreateSubscription(TopicType_Bbgid, bbgid)
}

// Specializated subscription remove function for tickers
func (ctx ManagedContext) RemoveTicker(ticker string) error {
	return ctx.RemoveSubscription(TopicType_Ticker, ticker)
}

// Specializated subscription remove function for Bbgids
func (ctx ManagedContext) RemoveBbgid(bbgid string) error {
	return ctx.RemoveSubscription(TopicType_Bbgid, bbgid)
}

// Specializated unsubscription function for tickers
func (ctx *ManagedContext) UnsubscribeTicker(ticker string) error {
	return ctx.CancelSubscription(TopicType_Ticker, ticker)
}

// Specializated unsubscription function for Bbgids
func (ctx *ManagedContext) UnsubscribeBbgid(bbgid string) error {
	return ctx.CancelSubscription(TopicType_Bbgid, bbgid)
}

// Specializated function to the retrieve the list of subscribed tickers
func (ctx ManagedContext) GetSubscribedTickers() []string {
	return ctx.GetSubscribedTopics(TopicType_Ticker)
}

// Specializated function to the retrieve the list of subscribed bbgids
func (ctx ManagedContext) GetSubscribedBbgids() []string {
	return ctx.GetSubscribedTopics(TopicType_Bbgid)
}

func (ctx ManagedContext) GetTickerCorrelationId(ticker string) (uint64, error) {
	return ctx.GetCorrelationId(TopicType_Ticker, ticker)	
}

func (ctx ManagedContext) GetBbgidCorrelationId(bbgid string) (uint64, error) {
	return ctx.GetCorrelationId(TopicType_Bbgid, bbgid)
}
