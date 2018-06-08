package main

import (
    "fmt"
    "os"
    "strings"
)

/*
s := ""
var s string
var s = ""
var s string = ""
*/

func main() {
    var s, sep string
    for i := 1; i < len(os.Args); i++ {
        s += sep + os.Args[i]
        sep = " "
    }
    fmt.Println(s)

    s, sep = "", ""
    for _, arg := range os.Args[1:] {
        s += sep + arg
        sep = " "
    }
    fmt.Println(s)

    fmt.Println(strings.Join(os.Args[1:], " "))
    fmt.Println(os.Args[1:])
}