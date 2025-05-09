#!/bin/bash
mkdir -p bin
for dir in cmd/*; do
    app=$(basename "$dir")
    go build -o "bin/$app" "./cmd/$app"
done
