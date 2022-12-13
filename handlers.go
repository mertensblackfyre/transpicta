package main

import (
	"net/http"

	"github.com/gorilla/mux"
)

func MakeHTTPHandler(f APIFunc) http.HandlerFunc {
	return func(w http.ResponseWriter, r *http.Request) {
		if err :=f(w,r);err!=nil{
			if e,ok := err.(APIError);ok {
				WriteJSON(w,e.Status,e)
				return
			}

			WriteJSON(w,http.StatusInternalServerError,
				APIError{
					Err:"Internal Server Error",
			})
		}
	}
}

func GetAllMoviesHandler(w http.ResponseWriter, r *http.Request) error {
	params := mux.Vars(r)
	if r.Method != http.MethodGet {
		return APIError{Err: "Invalid Method", Status:http.StatusMethodNotAllowed}
	}

	movies := Scraper("movie",params["limit"])

	return WriteJSON(w,http.StatusOK,movies)
}

func GetAllShowsHandler(w http.ResponseWriter, r *http.Request) error {
	params := mux.Vars(r)
	if r.Method != http.MethodGet {
		return APIError{Err: "Invalid Method", Status:http.StatusMethodNotAllowed}
	}

	shows := Scraper("tv",params["limit"])

	return WriteJSON(w,http.StatusOK,shows)
}


func GetMovieByNameHandler(w http.ResponseWriter, r *http.Request) error {
	// params := mux.Vars(r)
	var items []Result

	if r.Method != http.MethodGet {
		return APIError{Err: "Invalid Method", Status:http.StatusMethodNotAllowed}
	}

	return WriteJSON(w,http.StatusOK,items)
}


func GetShowByNameHandler(w http.ResponseWriter, r *http.Request) error {
	// params := mux.Vars(r)
	var items []Result
	
	if r.Method != http.MethodGet {
		return APIError{Err: "Invalid Method", Status:http.StatusMethodNotAllowed}
	}

	return WriteJSON(w,http.StatusOK,items)
}