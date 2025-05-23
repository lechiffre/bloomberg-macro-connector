package main

import (
	"blpconngo"
	"log"
	"time"
	_ "fmt"
)

func main() {
	ctx := blpconngo.NewContext()
	ctx.AddNotificationHandler(blpconngo.Callback)
	configPath := "./config.json"
	res := ctx.InitializeSession(configPath)
	if !res {
		log.Fatal("Failed to initialize session")
	}
	defer ctx.ShutdownSession()
	request := blpconngo.NewSubscriptionRequest()
	request.SetTopic("INJCJC Index")
	request.SetSubscription_type(blpconngo.SubscriptionType_ReleaseCalendar)
	request.SetCorrelation_id(4)
	ctx.Subscribe(request)
	time.Sleep(10 * time.Second)
	ctx.Unsubscribe(request)
	time.Sleep(3 * time.Second)
}
