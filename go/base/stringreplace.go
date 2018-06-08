package main  
  
import (  
    "fmt"  
    "strings"  
)  
  
func main() {  
    str := "welcome to bai\ndu\n.com"  
    // 去除空格  
    str = strings.Replace(str, " ", "", -1)  
    // 去除换行符  
    str = strings.Replace(str, "\n", "", -1)  
    fmt.Println(str)  
}  