package main

import "os"
import "fmt"

func main() {
	//os.Args provides access to raw command-line arguments.
	//Note that the first value in this slice is the path to the program, and os.Args[1:] holds the arguments to the program.
    argsWithProg := os.Args
    argsWithoutProg := os.Args[1:]

	//You can get individual args with normal indexing.
    arg := os.Args[3]
    fmt.Println(argsWithProg)
    fmt.Println(argsWithoutProg)
    fmt.Println(arg)
}

/*
package main

import (
    "fmt"
    "os"
)

func main() {
    var s, sep string
    for i := 1; i < len(os.Args); i++ {
        s += sep + os.Args[i]
        sep = " "
    }
    fmt.Println(s)

    fmt.Println(strings.Join(os.Args[1:], " "))
}
*/