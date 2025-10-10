all:
	sh install.sh

setup:
	go install golang.org/x/tools/cmd/stringer@latest
	stringer -type=ModuleType
	stringer -type=SessionStatus 
	stringer -type=SubscriptionStatus 
	stringer -type=ServiceStatus
	stringer -type=ReleaseStatus
	stringer -type=EventSubType
	stringer -type=EventType

