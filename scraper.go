package main

import (
	"fmt"

	"github.com/gocolly/colly"
)



func main(){
	c := colly.NewCollector(colly.AllowedDomains("en.wikipedia.org"))

	// fmt.Println("H")
	c.OnError(func(r *colly.Response, e error) {
		fmt.Printf("Error while scraping: %s\n", e.Error())
	})


	c.OnHTML("table:firstchild",func(e *colly.HTMLElement) {
		fmt.Println(e.Text)
		// e.ForEach("tr",func(i int, el *colly.HTMLElement) {
		// 	fmt.Println(el.Text)
		// 	data_slice := []string{}
		// 	el.ForEach("td",func(i int, h *colly.HTMLElement) {
		// 		data_slice = append(data_slice, h.Text)
		// 		fmt.Println(h.Text)
		// 	})
		// })
	})

	c.Visit("https://en.wikipedia.org/wiki/List_of_The_Simpsons_episodes")

}