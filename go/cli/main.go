package main

import (
	"blpconngo"
	"fmt"
	"log"
	"os"
	"strconv"
	"strings"

	"github.com/chzyer/readline"
)

const (
	SUBSCRIBE   = "subscribe"
	UNSUBSCRIBE = "unsubscribe"
)

func eventRequest(ctx blpconngo.Context, subscriptionType blpconngo.BlpConnSubscriptionType, corr_id uint64, topic string, action string) {
	request := blpconngo.NewSubscriptionRequest()
	request.SetTopic(topic)
	request.SetSubscription_type(subscriptionType)
	request.SetCorrelation_id(corr_id)

	if action == SUBSCRIBE {
		ctx.Subscribe(request)
	} else if action == UNSUBSCRIBE {
		ctx.Unsubscribe(request)
	}
}

func processCommand(ctx blpconngo.Context, line string) {
	parts := strings.Fields(line)
	if len(parts) < 4 {
		fmt.Println("Invalid command format")
		return
	}

	cmd, evt, corrStr, topic := parts[0], parts[1], parts[2], strings.Join(parts[3:], " ")
	corrID, err := strconv.ParseUint(corrStr, 10, 64)
	if err != nil {
		fmt.Println("Invalid correlation ID")
		return
	}

	switch cmd {
	case SUBSCRIBE:
		if evt == "calendar" {
			eventRequest(ctx, blpconngo.SubscriptionType_ReleaseCalendar, corrID, topic, SUBSCRIBE)
		} else if evt == "economic" {
			eventRequest(ctx, blpconngo.SubscriptionType_HeadlineActuals, corrID, topic, SUBSCRIBE)
		} else {
			fmt.Println("Undefined event type")
		}
	case UNSUBSCRIBE:
		if evt == "calendar" {
			eventRequest(ctx, blpconngo.SubscriptionType_ReleaseCalendar, corrID, topic, UNSUBSCRIBE)
		} else if evt == "economic" {
			eventRequest(ctx, blpconngo.SubscriptionType_HeadlineActuals, corrID, topic, UNSUBSCRIBE)
		} else {
			fmt.Println("Undefined event type")
		}
	default:
		fmt.Println("Undefined command")
	}
}

func run(ctx blpconngo.Context) {
	historyPath := os.Getenv("HOME") + "/.blpconn_history"
	rl, err := readline.NewEx(&readline.Config{
		Prompt:          "> ",
		HistoryFile:     historyPath,
		HistoryLimit:    1000,
		InterruptPrompt: "^C",
		EOFPrompt:       "exit",
	})
	if err != nil {
		log.Fatalf("Failed to initialize readline: %v", err)
	}
	defer rl.Close()

	for {
		line, err := rl.Readline()
		if err != nil { // Handle EOF or interrupt
			break
		}
		line = strings.TrimSpace(line)
		if line == "quit" || line == "exit" {
			break
		}
		if line != "" {
			rl.SaveHistory(line)
		}
		processCommand(ctx, line)
	}
}

func main() {
	ctx := blpconngo.NewContext()
	ctx.AddNotificationHandler(blpconngo.Callback)
	configPath := "./config.json"
	if !ctx.InitializeSession(configPath) {
		log.Fatal("Failed to initialize session")
	}
	defer ctx.ShutdownSession()
	run(ctx)
}
