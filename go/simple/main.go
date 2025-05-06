package main

/*
#include <stdlib.h>
*/
import "C"

import (
	"blpconngo"
	"log"
	"time"
)

func main() {
	ctx := blpconngo.NewContext()
	configPath := "/home/jailop/devel/li/bloomberg-geco-connector/go/simple/config.json"
	res := ctx.InitializeService(&configPath)
	if !res {
		log.Fatal("Failed to initialize service")
	}
	request := blpconngo.NewSubscriptionRequest()
	request.SetTopic("CATBTOTB Index")
	ctx.Subscribe(request)
	time.Sleep(10 * time.Second)
	ctx.ShutdownService()
}
