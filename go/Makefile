all:
	GOBIN=${PWD}/bin go install ./simple
	GOBIN=${PWD}/bin go install ./cli
	GOBIN=${PWD}/bin go install ./managed

setup:
	go install golang.org/x/tools/cmd/stringer@latest
	go get golang.org/x/tools/cmd/godoc
	stringer -type=ModuleType
	stringer -type=SessionStatus 
	stringer -type=SubscriptionStatus 
	stringer -type=ServiceStatus
	stringer -type=ReleaseStatus
	stringer -type=EventSubType
	stringer -type=EventType
	go get blpconngo
	go get blpconngo/cli
	go get blpconngo/simple
	go get blpconngo/managed

test:
	go test ./tests/deserialize_test.go
	go test ./tests/referencemap_test.go

doc:
	go doc --all > API.txt

