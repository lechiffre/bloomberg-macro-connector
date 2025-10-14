package main

import (
	"blpconngo"
	"time"
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
	ctx.SubscribeTicker("INJCJC Index")
	time.Sleep(10 * time.Second)
}
