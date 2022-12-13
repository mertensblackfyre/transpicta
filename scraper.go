package main

import (
	"encoding/json"
	"fmt"
	"log"
	"os"
	"strings"

	"github.com/gocolly/colly"
)

type Result struct {
	Title string 	`json:"title"`
	Quality string	`json:"quality"`
	IMDB string `json:"imdb"`
	Description string `json:"description"`
	Genre []string 	`json:"genre"`
	Image string `json:"image"`
}

func Scraper(category string,limit string) []Result{
	c := colly.NewCollector(colly.AllowedDomains("sflix.to"))
	detailCollector := c.Clone()
	var items []Result

	c.OnError(func(r *colly.Response, e error) {
		fmt.Printf("Error while scraping: %s\n", e.Error())
	})

	c.OnHTML("a[title]", func(h *colly.HTMLElement) {
		movieURL := h.Request.AbsoluteURL(h.Attr("href"))

		if strings.Contains(movieURL, "/tv/"){
			detailCollector.Visit(movieURL)
		}
	})

	detailCollector.OnHTML(`div[class=container] div.detail_page-infor div.dp-i-content`, func(h *colly.HTMLElement) {
	
		if  h.ChildText("h2.heading-name") == "" {
			log.Println("No title found", h.Request.URL)
		}
		item := Result{
			Title:  h.ChildText("h2.heading-name"),
			Quality:  h.ChildText("div.film-stats span.quality"),
			IMDB:  strings.Trim(h.ChildText("div.film-stats span.imdb"),"IMDB: "),
			Description: strings.Trim(h.ChildText("div.description"),"\nOverview:"),
			Image: h.ChildAttr("img","src"),
		}

		h.ForEach("a[title]", func(_ int, el *colly.HTMLElement) {
			L := el.Request.AbsoluteURL(el.Attr("href"))
			if strings.Contains(L, "/genre"){
				new := strings.Trim(L, "https://sflix.to/genre/")
				item.Genre = append(item.Genre,new)
			}
		})
		items = append(items, item)
	})

	c.OnHTML("[title=Next]",func(h *colly.HTMLElement) {
		next_page := h.Request.AbsoluteURL(h.Attr(("href")))
		if next_page != ("https://sflix.to/top-imdb?type=" + category + "&page=" + limit) {
			c.Visit(next_page)
		}	
		fmt.Println("You have reached the limit")
	})
	
	c.OnRequest(func(r *colly.Request) {
		r.Headers.Set("Accept-Language", "en-US;q=0.9")
		fmt.Printf("Visiting: %s\n", r.URL)		
	})


	c.Visit("https://sflix.to/top-imdb?type=" + category)

	content,err := json.Marshal(items)
		if err != nil {
			fmt.Println(err.Error())
		}

	os.WriteFile("shows.json",content,0644)
	return items
}