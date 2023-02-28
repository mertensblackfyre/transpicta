package main

import (
	"flag"
	"fmt"
	"log"

	"net/http"
)

func main() {

	port := flag.String("port", ":3000", "Port number")
	flag.Parse()

	mux := DefaultMux()

	ya, err := YAMLHandler("url.yaml", mux)
	if err != nil {
		log.Fatalln(err)
	}

	log.Println("Server running on port ", *port)
	http.ListenAndServe(*port, ya)
}

func DefaultMux() *http.ServeMux {
	mux := http.NewServeMux()
	mux.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
		fmt.Println("Hello World")
	})

	return mux
}
