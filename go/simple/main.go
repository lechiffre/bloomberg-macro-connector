package main

import (
	"blpconngo"
	"log"
	"time"
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
	ctx.Subscribe(request)
	time.Sleep(10 * time.Second)
}
