package main

import (
	"compress/gzip"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"time"
)

type Config struct {
	Enable int `json:"enable"`
	Mode   int `json:"mode"`
}

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

func handleGetConfig(w http.ResponseWriter, r *http.Request) {
	fmt.Println(time.Now().String(), r.URL.Path)

	config := Config{
		Enable: 1,
		Mode:   2,
	}
	jsonData, err := json.Marshal(config)
	if err != nil {
		fmt.Println("JSON encoding error:", err)
		return
	}

	w.Header().Set("Content-Type", "application/json")
	w.Write(jsonData)
}

func main() {
	http.Handle("/", &TestHandler{"Hi"})
	http.HandleFunc("/test", SayHello)
	http.HandleFunc("/config", handleGetConfig)
	http.HandleFunc("/report/error/v2", testPost)
	http.Handle("/files/", http.StripPrefix("/files/", http.FileServer(http.Dir("./"))))
	http.ListenAndServe("192.168.1.129:8001", nil)
}
