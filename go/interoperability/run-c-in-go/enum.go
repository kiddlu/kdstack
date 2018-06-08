package main

// enum color {
//    RED = 6,
//    BLUE,
//    YELLOW
// };
import "C"

import "fmt"
 
var e, f, g C.enum_color = C.RED, C.BLUE, C.YELLOW

func main() {
    fmt.Println(e, f, g)
}