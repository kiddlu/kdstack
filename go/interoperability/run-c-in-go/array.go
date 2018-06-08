package main

// int cArray[] = {1, 2, 3, 4, 5, 6, 7};
import "C"

import "unsafe"

import "fmt"

func CArrayToGoArray(cArray unsafe.Pointer, size int) (goArray []int) {
    p := uintptr(cArray)
    for i :=0; i < size; i++ {
        j := *(*int)(unsafe.Pointer(p))
        goArray = append(goArray, j)
        p += unsafe.Sizeof(j)
    }
 
    return
}
 
func main() {
    goArray := CArrayToGoArray(unsafe.Pointer(&C.cArray[0]), 7)
    fmt.Println(goArray)
}