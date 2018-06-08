package main

import "fmt"
import "io/ioutil"
import "net/http"

func main() {
	resp, err := http.Get("https://www.kernel.org/")
	if err == nil {
		defer resp.Body.Close()
		body, err := ioutil.ReadAll(resp.Body)
		if err == nil {
			fmt.Println(string(body))
		}
	}
}