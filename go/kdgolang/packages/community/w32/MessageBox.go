package main

import "os"
import "fmt"
import "github.com/AllenDang/w32"

func main() {
	arg_num := len(os.Args)
	if arg_num < 3 {
		fmt.Println("Useage: MessageBox text title")
	}  else {
		w32.MessageBox(0, os.Args[1], os.Args[2], w32.MB_ICONINFORMATION|w32.MB_OK)
	}
}
