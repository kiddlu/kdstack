package main						
                                                                                                                                
import (							
   "fmt"							
)								   
                                                                                                                                
var a string						
var c = make(chan int, 10)		  
                                                                                                                                
func f() {						  
  fmt.Println("f 函数运行了")	 
  a = "hello, world"			  
  c <- 0						  
}								   
                                                                                                                                
func main() {					   
  fmt.Println("先让f这个函数先跑着")
  go f()						  
  <-c							 
  fmt.Println("我这里取得了a的值",a)
}