package main
 
// #include <stdio.h>
// #include <stdlib.h>
// char *string = "Hello C String";
/*
void print(char *str) {
    printf("%s\n", str);
}
*/
import "C"
 
import "unsafe"
import "fmt"
 
func main() {
	//Go string to C string
    s := "Hello Go string"
    cs := C.CString(s)
    C.print(cs)
    C.free(unsafe.Pointer(cs))

    //C string to Go string
    fmt.Printf("%s\n", C.GoString(C.string))
}