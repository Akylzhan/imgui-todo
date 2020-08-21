package main

// #include <stdlib.h>
import "C"
import "unsafe"
import (
  "time"
)

//export ShowDate
func ShowDate(y, m, d int) (int, *C.char, int){
  ny, nm, nd := time.Date(y, time.Month(m), d, 0, 0, 0, 0, time.UTC).Date()
  cs := C.CString(nm.String())
  C.free(unsafe.Pointer(cs))
  return ny, cs, nd
}

//export CurrentYearMonthDay
func CurrentYearMonthDay() (int, int, int) {
  y, m, d := time.Now().Date()
  return y, int(m), d
}

func main() {}