#!/usr/bin/bash
cd c
gcc -shared -o libapi.so api.c
cp api.h ../

cd ..
go build -buildmode=c-shared -o libchttpd.so gx.io/cgo/chttpd

cd c
gcc -o chttpd  main.c -L. -L.. -lpthread -lapi -lchttpd
mv chttpd ../

cd ..
LD_LIBRARY_PATH=.:c ./chttpd