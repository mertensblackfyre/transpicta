package main

import (
	"crypto/rand"
	"encoding/hex"
	"encoding/json"
	"net/http"
)


func GenerateSecureToken(length int) string {
    b := make([]byte, length)

    if _, err := rand.Read(b); err != nil {
        return ""
    }
    return hex.EncodeToString(b)
}


func WriteJSON(w http.ResponseWriter, status int,v any) error {
	w.WriteHeader(status)
	w.Header().Add("Content-Type","applictaion/json")
	return json.NewEncoder(w).Encode(v)
}