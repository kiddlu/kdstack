package main

import "fmt"

/*
for initialization; condition; post {
    // zero or more statements
}

for condition {
    //a traditional "while" loop
}

for {
    //a traditional infinite loop
    //可以靠break或者return语句来终止掉循环
}

func main() {
    s, sep := "", ""
    for _, arg := range os.Args[1:] {
        s += sep + arg
        sep = " "
    }
    fmt.Println(s)
}

*/


func main() {

	//The most basic type, with a single condition.
    i := 1
    for i <= 3 {
        fmt.Println(i)
        i = i + 1
    }

	//A classic initial/condition/after for loop.
    for j := 7; j <= 9; j++ {
        fmt.Println(j)
    }

	//for without a condition will loop repeatedly
	//until you break out of the loop or return from the enclosing function.
    for {
        fmt.Println("loop")
        break
    }
}