#!/bin/bash

platform=`uname`
if [[ "$platform" == 'Linux' ]]; then
    rm -rf build/ && mkdir build && cd build && cmake .. && make && cd ..
fi
