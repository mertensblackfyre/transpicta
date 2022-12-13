package main

import "net/http"


type APIError struct{
	Err string
	Status int
}
type APIFunc func(http.ResponseWriter,*http.Request) error

func (e APIError) Error() string {
	return e.Err
}
