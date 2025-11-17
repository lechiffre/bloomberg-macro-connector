package main

import (
	"blpconngo"
	"time"
	"fmt"
	"log"
)

func main() {
	ctx := blpconngo.NewManagedContext()
	configPath := "./config.json"
	ctx.AddNotificationHandler(blpconngo.Callback)
	if ok := ctx.InitializeSession(configPath); !ok {
		log.Fatal("Failed to initialize session")
	}
	defer ctx.ManagedShutdown()
	// corrId1, _ := ctx.SubscribeTicker("CACPMYOY Index");
	// corrId2, _ := ctx.SubscribeTicker("CACPTYOY Index");
	corrId1, _ := ctx.CreateSubscription(blpconngo.TopicType_Ticker, "CACPMYOY Index")
	corrId2, _ := ctx.CreateSubscription(blpconngo.TopicType_Ticker, "CACPTYOY Index")
	if corrId1 == corrId2 {
		fmt.Println("error: Both correlation IDs are the same!")
	} else {
		fmt.Println("success: Correlation IDs are different")
	}
	time.Sleep(10 * time.Second)
}
