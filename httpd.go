package main

/*
#cgo LDFLAGS: -L./c -lapi
#include <unistd.h>
#include "api.h"
#include "stdlib.h"
*/
import "C"
import (
	"fmt"
	"io"
	"log"
	"net/http"
	"text/template"
	"unsafe"

	"github.com/julienschmidt/httprouter"
)

func main() {
	//start_httpd(":8080")
}

//export start_httpd
func start_httpd(addr string) {
	go func() {
		router := httprouter.New()
		router.GET("/", func(w http.ResponseWriter, r *http.Request, _ httprouter.Params) {
			t := template.Must(template.ParseFiles("template/index.html"))
			t.Execute(w, [...]string{"Basic routing support", "Support regex", "High performance", "Good documentation"})
		})

		router.GET("/set", func(w http.ResponseWriter, r *http.Request, _ httprouter.Params) {
			v := C.set()
			fmt.Printf("%T, %T, %v\n", v, int(v), v)
			io.WriteString(w, "You requested /set")
		})

		router.GET("/get", func(w http.ResponseWriter, r *http.Request, _ httprouter.Params) {
			v := C.get_vars()
			//fmt.Printf("%T, %T, %v\n", v, C.GoString(v), C.GoString(v))
			w.Header().Add("Content-Type", "application/json")
			w.Write([]byte(C.GoString(v)))
			C.free(unsafe.Pointer(v))
		})

		if err := http.ListenAndServe(addr, router); err != nil {
			log.Fatal(err)
		}
	}()

	fmt.Println("chttpd started @" + addr)
}
