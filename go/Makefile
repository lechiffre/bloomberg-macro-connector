all:
	GOBIN=${PWD}/bin go install ./simple
	GOBIN=${PWD}/bin go install ./cli
	GOBIN=${PWD}/bin go install ./example

setup:
	go install golang.org/x/tools/cmd/stringer@latest
	stringer -type=ModuleType
	stringer -type=SessionStatus 
	stringer -type=SubscriptionStatus 
	stringer -type=ServiceStatus
	stringer -type=ReleaseStatus
	stringer -type=EventSubType
	stringer -type=EventType

test:
	go test ./tests/deserialize_test.go
