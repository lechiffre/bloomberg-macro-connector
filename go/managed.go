package blpconngo

import (
	"sync"
	"fmt"
)

type ManagedContext struct {
	Context
	subscriptions map[string]SubscriptionRequest
	nextCorrId	  uint64
	mu			  sync.Mutex
}

func NewManagedContext() ManagedContext {
	return ManagedContext{
		Context: NewContext(),
		subscriptions: make(map[string]SubscriptionRequest),
		nextCorrId: 1,
	}
}

func (ctx *ManagedContext) ManagedSubscribe(topicType BlpConnTopicType, instrument string) error {
	ctx.mu.Lock()
	defer ctx.mu.Unlock()
	if _, exists := ctx.subscriptions[instrument]; exists {
		return fmt.Errorf("%s already subscribed", instrument)
	}
	request := NewSubscriptionRequest()
	request.SetTopic(instrument)
	request.SetTopic_type(topicType)
	request.SetCorrelation_id(ctx.nextCorrId)
	ctx.nextCorrId += 1
	ctx.Subscribe(request)
	ctx.subscriptions[instrument] = request
	return nil
}

func (ctx *ManagedContext) ManagedUnsubscribe(topicType BlpConnTopicType, instrument string) error {
	ctx.mu.Lock()
	defer ctx.mu.Unlock()
	request, exists := ctx.subscriptions[instrument]
	if !exists {
		return fmt.Errorf("%s is not subscribed", instrument)
	}
	ctx.Unsubscribe(request)
	delete(ctx.subscriptions, instrument)
	return nil
}

func (ctx *ManagedContext) SubscribeTicker(ticker string) error {
	return ctx.ManagedSubscribe(TopicType_Ticker, ticker)
}

func (ctx *ManagedContext) SubscribeBbgid(bbgid string) error {
	return ctx.ManagedSubscribe(TopicType_Bbgid, bbgid)
}

func (ctx *ManagedContext) UnsubscribeTicker(ticker string) error {
	return ctx.ManagedUnsubscribe(TopicType_Ticker, ticker)
}

func (ctx *ManagedContext) UnsubscribeBbgid(bbgid string) error {
	return ctx.ManagedUnsubscribe(TopicType_Bbgid, bbgid)
}

func (ctx ManagedContext) ManagedSubscribed(topicType BlpConnTopicType) []string {
	keys := make([]string, 0)
	for _, subs := range ctx.subscriptions {
		if subs.GetTopic_type() == topicType {
			keys = append(keys, subs.GetTopic())
		}
	}
	return keys
}

func (ctx ManagedContext) SubscribedTickers() []string {
	return ctx.ManagedSubscribed(TopicType_Ticker)
}

func (ctx ManagedContext) SubscribedBbgids() []string {
	return ctx.ManagedSubscribed(TopicType_Bbgid)
}
