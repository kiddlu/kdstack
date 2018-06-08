package main

import (
	"syscall"
)

func callDll() {
	dll32 := syscall.NewLazyDLL("example.dll")
	println("call dll:", dll32.Name)

	gcd := dll32.NewProc("gcd")
	ret, _, _ := gcd.Call(uintptr(4), uintptr(8))

	println()
	println("get the result:", ret)
}

func main() {
	callDll()
}