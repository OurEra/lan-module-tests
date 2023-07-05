package main

import (
	"fmt"
	"log"
	"net/http"
	"time"

	// "encoding/json"
	"compress/gzip"
	"io/ioutil"
)

type TestHandler struct {
	str string
}

func SayHello(w http.ResponseWriter, r *http.Request) {
	log.Printf("HandleFunc")
	w.Write([]byte(string("HandleFunc")))
}

// ServeHTTP方法，绑定TestHandler
func (th *TestHandler) ServeHTTP(w http.ResponseWriter, r *http.Request) {
	log.Printf("Handle")
	w.Write([]byte(string("Handle")))
}

func testPost(w http.ResponseWriter, r *http.Request) {

	body, err := gzip.NewReader(r.Body)
	if err != nil {
		fmt.Println("unzip is failed, err:", err)
	}
	defer body.Close()
	data, err := ioutil.ReadAll(body)
	if err != nil {
		fmt.Println("-------------read all is failed.err:", err)
	}
	fmt.Printf("----URL----%v\n", r.URL)
	fmt.Println("%s string(data)=%s", time.Now().String(), string(data))

	//decoder := json.NewDecoder(data)
	//var params map[string]string
	//decoder.Decode(&params)
	//fmt.Println(params)

	fmt.Fprintf(w, `{"code":0}`)
}

func main() {
	http.Handle("/", &TestHandler{"Hi"})
	http.HandleFunc("/test", SayHello)
	http.HandleFunc("/report/error/v2", testPost)
	http.Handle("/files/", http.StripPrefix("/files/", http.FileServer(http.Dir("./"))))
	http.ListenAndServe("100.100.57.138:8000", nil)
}
