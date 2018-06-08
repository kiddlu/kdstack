/*
Serve is a very simple static file server in go
Usage:
	-p="8080": port to serve on
	-d=".":    the directory of static files to host
Navigating to http://localhost:8080 will display the index.html or directory
listing file.
*/

// from https://gist.github.com/paulmach/7271283

package main

import (
	"flag"
	"log"
	"net/http"
)

func main() {
	port := flag.String("p", "8080", "port")
	dir := flag.String("d", ".", "main dir")
	flag.Parse()

	http.Handle("/", http.FileServer(http.Dir(*dir)))

	log.Printf("Starting goweb on HTTP port: %s, path %s\n", *port, *dir)
	log.Fatal(http.ListenAndServe(":"+*port, nil))
}
