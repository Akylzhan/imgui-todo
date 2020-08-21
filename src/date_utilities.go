package main

// #include <stdlib.h>
import "C"
import (
  "time"
)

//export ShowDate
func ShowDate(y, m, d int) (int, int, int){
  ny, nm, nd := time.Date(y, time.Month(m), d, 0, 0, 0, 0, time.UTC).Date()
  return ny, int(nm), nd
}

//export CurrentYearMonthDay
func CurrentYearMonthDay() (int, int, int) {
  y, m, d := time.Now().Date()
  return y, int(m), d
}

func main() {}