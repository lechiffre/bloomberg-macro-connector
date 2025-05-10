package main

import (
	root "blpconngo"
	"fmt"
	"io"
	"os"
)

func deserializeFromFile(filePath string) {
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
	root.NativeHandler(data)
}

func main() {
	if len(os.Args) < 2 {
		fmt.Println("Please provide a file path as an argument.")
		return
	}
	filePath := os.Args[1]
	deserializeFromFile(filePath)
}
