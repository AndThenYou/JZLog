#!/bin/bash

workdir=$(cd $(dirname $0); pwd)
cd ./glog

chmod +x ./autogen.sh
./autogen.sh
./configure CPPFLAGS=-fPIC --prefix=$workdir --enable-static --disable-shared
make
make install

