package main

import (
	"encoding/csv"
	"flag"
	"fmt"
	"log"
	"os"
	"time"
)

func main() {

	QA := [][]string{}
	
	score := 0

	filename := flag.String("csv", "problems.csv", "The filename of the csv")
	timeout := flag.Int("time", 0, "The number of seconds for the timer")

	if *timeout < 0 {
		flag.PrintDefaults()
		os.Exit(1)
	}

	flag.Parse()

	timer := time.NewTimer(time.Duration(*timeout) * time.Second)

	file, err := os.Open(*filename)
	if err != nil {
		log.Fatalln(err)
	}

	r := csv.NewReader(file)
	defer file.Close()

	QA, err1 := r.ReadAll()
	if err1 != nil {
		log.Fatalln(err)
	}

	for k, v := range QA {
		fmt.Printf("Problem# %d: What %s, sir? ", k, v[0])
		answer_channel := make(chan string)
		go func() {
			var answer string
			fmt.Scanf("%s\n", &answer)
			answer_channel <- answer
		}()
		select {
		case <-timer.C:
			fmt.Println("\nTimes up!")
			fmt.Printf("Your score: %d\n", score)
			return
		case answer := <-answer_channel:
			if answer != v[1] {
				fmt.Printf("Wrong. Correct answer %s\n", v[1])
				fmt.Printf("Your score: %d\n", score)
				break
			}
			fmt.Println("Correct Answer")
			score++
		}
	}

	if score == len(QA) {
		fmt.Println("You answerd all questions!")
		fmt.Printf("Your score: %d\n", score)
	}
}
