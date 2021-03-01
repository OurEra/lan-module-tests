package main

import (
	"fmt"
)

type person struct {
	name string
	age  int
}

func (p person) test_method() {
	fmt.Println("method of person")
}

func sum(a int, b int) (int, error) {
	return a + b, nil
}

func main() {
	fmt.Println("entry")

	_person := person{"me", 30}
	_person.test_method()
	fmt.Println(_person)
}
