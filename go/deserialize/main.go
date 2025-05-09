package main

import (
	"os"
	"io"
	"fmt"
	root "blpconngo"
)
func main() {
	if len(os.Args) < 2 {
		fmt.Println("Usage: go run main.go <path_to_flatbuffer_file>")
		return
	}
	filePath := os.Args[1]
	file, err := os.Open(filePath)
	if err != nil {
		fmt.Printf("Error opening file: %v\n", err)
		return
	}
	defer file.Close()
	data, err := io.ReadAll(file)
	if err != nil {
		fmt.Printf("Error reading file: %v\n", err)
		return
	}
	// Pass data to NativeHandler
	root.NativeHandler(data)
}
