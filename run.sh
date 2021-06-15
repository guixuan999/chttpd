#!/usr/bin/bash

# build libcjson.so
cd c/cJSON
mkdir build
cd build
cmake ..
make

# build libapi.so
cd ../../
mv cJSON/build/libcjson.so* ./
gcc -shared -o libapi.so api.c -I. -L. -lcjson
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