package main

/*
#cgo LDFLAGS: -L./c -lapi
#include <unistd.h>
#include "api.h"
#include "stdlib.h"
*/
import "C"
import (
	"encoding/json"
	"fmt"
	"io"
	"log"
	"net/http"
	"text/template"
	"unsafe"

	"github.com/julienschmidt/httprouter"
)

type debug_var struct {
	Src_filename string
	Var_name     string
	Var_type     string
	Var          interface{}
}

type debug_vars []debug_var

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

		router.POST("/set", func(w http.ResponseWriter, r *http.Request, _ httprouter.Params) {
			in := make([]byte, r.ContentLength)
			n, err := r.Body.Read(in)
			if int64(n) != r.ContentLength || (err != nil && err != io.EOF) {
				w.WriteHeader(501)
				io.WriteString(w, "Error while read request body")
				return
			}

			var vars debug_vars
			err = json.Unmarshal(in, &vars)
			if err != nil {
				w.WriteHeader(501)
				io.WriteString(w, "Error json.Unmarshal")
				return
			}

			c := C.CString(string(in))
			defer C.free(unsafe.Pointer(c))
			v := C.set_vars(c)
			if int(v) == 1 {
				fmt.Printf("C.set_vars returns %d\n", v)
			} else {
				w.WriteHeader(501)
				io.WriteString(w, "Fail")
				return
			}
			io.WriteString(w, "OK")
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
