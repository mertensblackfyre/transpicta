package main

import (
	"flag"
	"fmt"
	"log"

	"net/http"
)

func main() {

	filename := flag.String("yml", "url.yaml", "The yaml file you want to read")
	flag.Parse()

	mux := DefaultMux()

	ya, err := YAMLHandler(*filename, mux)
	if err != nil {
		log.Fatalln(err)
	}

	log.Println("Server running on port 8000 ")
	http.ListenAndServe(":8000", ya)
}

func DefaultMux() *http.ServeMux {
	mux := http.NewServeMux()
	mux.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		fmt.Println("Hello World")
	})

	return mux
}
