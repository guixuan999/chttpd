#!/bin/bash

# build libcjson.so
f1=c/libcjson.so
f2=c/libcjson.so.1
f3=c/libcjson.so.1.7.14
if [ -f $f1 ] && [ -f $f2 ] && [ -f $f3 ]; then
    cd c/cJSON
    cd build
    echo "$f1 exists, skip cJSON building"
else
    cd c/cJSON
    mkdir build
    cd build
    cmake ..
    make
fi

# build libapi.so
cd ../../
mv cJSON/build/libcjson.so* ./
gcc -shared -o libapi.so api.c -I. -L. -lcjson -fPIC
cp api.h ../

# build libchttpd.so
cd ..
LD_LIBRARY_PATH=./c go build -buildmode=c-shared -o libchttpd.so gx.io/cgo/chttpd

# build chttpd (executable)
cd c
gcc -o chttpd  main.c -Wl,-rpath-link . -L. -L.. -lpthread -lapi -lchttpd
# or: LD_LIBRARY_PATH=. gcc -o chttpd  main.c -L. -L.. -lpthread -lapi -lchttpd
mv chttpd ../

# run chttpd
cd ..
LD_LIBRARY_PATH=.:c ./chttpd
