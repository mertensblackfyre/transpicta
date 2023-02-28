package main

import (
	"io/ioutil"
	"log"
	"net/http"

	yaml "gopkg.in/yaml.v3"
)

type PathURL struct {
	Path []string `yaml:"path"`
	URL  []string `yaml:"url"`
}

func JSONHandler() {}

func MapHandler(pathsToUrls map[string]string, fallback http.Handler) http.HandlerFunc {
	return http.HandlerFunc(func(w http.ResponseWriter, r *http.Request) {
		for k, v := range pathsToUrls {
			if r.URL.Path == k {
				http.Redirect(w, r, v, http.StatusSeeOther)
				return
			}
			fallback.ServeHTTP(w, r)
		}
	})
}

func YAMLHandler(filename string, fallback http.Handler) (http.HandlerFunc, error) {
	filebytes, err := ioutil.ReadFile(filename)
	paths := PathURL{}

	err = yaml.Unmarshal(filebytes, &paths)
	if err != nil {
		log.Fatalf("error: %v", err)
	}

	data := make(map[string]string)
	for k, v := range paths.Path {
		data[v] = paths.URL[k]
	}
	
	return MapHandler(data, fallback), err
}
