package main

/*
#cgo LDFLAGS: -L./c -lapi
#include <unistd.h>
#include "api.h"
*/
import "C"
import (
	"fmt"
	"io"
	"log"
	"net/http"
)

func main() {
	//start_httpd(":8080")
}

//export start_httpd
func start_httpd(addr string) {
	go func() {
		http.HandleFunc("/", func(w http.ResponseWriter, r *http.Request) {
			io.WriteString(w, "Now that you see this, everything looks fine.")
		})

		http.HandleFunc("/set", func(w http.ResponseWriter, r *http.Request) {
			v := C.set()
			fmt.Printf("%#V, %v\n", v, v)
			io.WriteString(w, "You requested /set")
		})

		if err := http.ListenAndServe(addr, nil); err != nil {
			log.Fatal("Can not start service")
		}
	}()

	fmt.Println("chttpd started @" + addr)
}
