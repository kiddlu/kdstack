package main
import "os"
import "strings"
import "fmt"
func main() {
	
	//To set a key/value pair, use os.Setenv. To get a value for a key, use os.Getenv. This will return an empty string if the key isn’t present in the environment.
    os.Setenv("FOO", "1")
    fmt.Println("FOO:", os.Getenv("FOO"))
    fmt.Println("TMP:", os.Getenv("TMP"))
	
	//Use os.Environ to list all key/value pairs in the environment. This returns a slice of strings in the form KEY=value. You can strings.Split them to get the key and value. Here we print all the keys.
    fmt.Println()
    for _, e := range os.Environ() {
        pair := strings.Split(e, "=")
        fmt.Println(pair[0])
    }
}