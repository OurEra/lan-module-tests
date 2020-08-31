package main

import (
	"fmt"
)

type person struct {
	name string
	age  int
}

func main() {
	fmt.Println("entry")

	_person := person{"me", 30}
	fmt.Println(_person)
}

func sum(a int, b int) (int, error) {
	return a + b, nil
}
