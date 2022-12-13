package main

import (
	"crypto/rand"
	"encoding/hex"
	"encoding/json"
	"fmt"
	"io/ioutil"
	"log"
	"net/http"
	"os"

	"github.com/gocolly/colly"
	"github.com/gorilla/mux"
)

type Result struct {
	Name string 	`json:"name"`
	Information string `json:"information"`
	Image string `json:"image"`
}

type Movies struct {
	Name string 	`json:"name"`
	Information string `json:"information"`
	Image string `json:"image"`
}

type APIError struct{
	Err string
	Status int
}
type APIFunc func(http.ResponseWriter,*http.Request) error

func (e APIError) Error() string {
	return e.Err
}


func ParseJSONFile(filename string) []Result {
	file, err := os.Open("shows.json")
	var movie []Result

	if err != nil {
	   log.Fatal(err)
	}

	log.Println("The file is opened successfully...")
	defer file.Close()
 
	bytes, _ := ioutil.ReadAll(file)
	json.Unmarshal([]byte(bytes), &movie)

	return movie
}

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

func Scraper(category string,limit string) []Movies{
	url := "https://sflix.to/top-imdb?type="+ category
	c := colly.NewCollector(colly.AllowedDomains("sflix.to"))

	var items []Movies

	c.OnError(func(r *colly.Response, e error) {
		fmt.Printf("Error while scraping: %s\n", e.Error())
	})

	c.OnHTML("div.film_list-wrap div[class=flw-item]", func(h *colly.HTMLElement) {
		item := Movies{
			Name: h.ChildText("h2.film-name"),
			Information:  h.ChildText("div.fd-infor span.fdi-item"),
			Image: h.ChildAttr("img","data-src"),
		}
			
		items  = append(items, item)
	})

	c.OnHTML("[title=Next]",func(h *colly.HTMLElement) {
		next_page := h.Request.AbsoluteURL(h.Attr(("href")))
		if next_page != (url + "&page=" + limit) {
			c.Visit(next_page)
		}	
		fmt.Println("You have reached the limit")
	})

	c.OnRequest(func(r *colly.Request){
		r.Headers.Set("Accept-Language", "en-US;q=0.9")

		fmt.Printf("Visiting: %s\n", r.URL)		
	})

	c.Visit(url)

	return items

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
	params := mux.Vars(r)

	if r.Method != http.MethodGet {
		return APIError{Err: "Invalid Method", Status:http.StatusMethodNotAllowed}
	}

	movies := Scraper("movie","")
	var items []Result

	for i := 0; i < len(movies); i++ {
		if movies[i].Name == params["name"] {
			fmt.Println(movies[i].Name)
			result := &Result{
				Name: movies[i].Name,
				Information: movies[i].Information,
				Image: movies[i].Image,
			}
			items = append(items, *result)
		} 
	}

	return WriteJSON(w,http.StatusOK,items)
}


func GetShowByNameHandler(w http.ResponseWriter, r *http.Request) error {
	params := mux.Vars(r)

	if r.Method != http.MethodGet {
		return APIError{Err: "Invalid Method", Status:http.StatusMethodNotAllowed}
	}

	shows := Scraper("tv","")
	var items []Result

	for i := 0; i < len(shows); i++ {
		if shows[i].Name == params["name"] {
			fmt.Println(shows[i].Name)
			result := &Result{
				Name: shows[i].Name,
				Information: shows[i].Information,
				Image: shows[i].Image,
			}
			items = append(items, *result)
		} 
	}

	return WriteJSON(w,http.StatusOK,items)
}

func main()  {
	routes := mux.NewRouter()
	routes.HandleFunc("/movies/limit={limit}",MakeHTTPHandler(GetAllMoviesHandler))
	routes.HandleFunc("/movie/{name}",MakeHTTPHandler(GetMovieByNameHandler))
	routes.HandleFunc("/shows/limit={limit}",MakeHTTPHandler(GetAllShowsHandler))
	routes.HandleFunc("/show/{name}",MakeHTTPHandler(GetShowByNameHandler))
	
	log.Fatal(http.ListenAndServe(":8000",routes))
}



