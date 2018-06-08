
package main
 
import (
    "fmt"
    "runtime"
)
 
func main() {
    fmt.Print("Go runs on ")
    //runtime.GOARCH
    os := runtime.GOOS
    switch os {
    case "darwin":
        fmt.Println("darwin")
    case "linux":
        fmt.Println("linux")
    case "windows":
        fmt.Println("windows")
    default:
        fmt.Printf("%s", os)
    }
}