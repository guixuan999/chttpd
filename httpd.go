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
	"text/template"

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
			fmt.Printf("%#V, %v\n", v, v)
			io.WriteString(w, "You requested /set")
		})

		router.GET("/get", func(w http.ResponseWriter, r *http.Request, _ httprouter.Params) {
			io.WriteString(w, "You requested /get")
		})

		if err := http.ListenAndServe(addr, router); err != nil {
			log.Fatal("Can not start service")
		}
	}()

	fmt.Println("chttpd started @" + addr)
}
