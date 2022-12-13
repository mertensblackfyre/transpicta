package main

import (
	"log"
	"net/http"

	"github.com/gorilla/mux"
)


func main()  {
	routes := mux.NewRouter()
	routes.HandleFunc("/movies/limit/{limit}",MakeHTTPHandler(GetAllMoviesHandler))
	routes.HandleFunc("/movie/{name}",MakeHTTPHandler(GetMovieByNameHandler))
	routes.HandleFunc("/shows/limit/{limit}",MakeHTTPHandler(GetAllShowsHandler))
	routes.HandleFunc("/show/{name}",MakeHTTPHandler(GetShowByNameHandler))
	
	log.Fatal(http.ListenAndServe(":8000",routes))
}