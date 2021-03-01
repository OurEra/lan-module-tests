package main

import (
	"fmt"
	"time"
)

func test_go() {
	for i := 0; i < 5; i++ {
		time.Sleep(100 * time.Millisecond)
		fmt.Println("test_go ", i)
	}

}

func main() {

	go test_go()
	time.Sleep(1000 * time.Millisecond)
}
