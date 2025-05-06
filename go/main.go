package main

/*
#cgo LDFLAGS: -L../lib -lblpapi3_64 -lblpconngo -lblpconn -lstdc++
#include <stdlib.h>
*/
import "C"

import (
	"gocli/blpconngo"
	"log"
	"time"
)

func main() {
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
	request.SetSubscription_type(blpconngo.SubscriptionType_ReleaseCalendar)
	ctx.Subscribe(request)
	time.Sleep(10 * time.Second)
}
