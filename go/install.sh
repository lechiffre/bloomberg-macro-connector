GOBIN=$PWD/bin go install ./simple
GOBIN=$PWD/bin go install ./cli
GOBIN=$PWD/bin go test ./tests/deserialize_test.go
