#!/bin/bash
export CC=/usr/bin/gcc-11 &&
export CXX=/usr/bin/g++-11 &&
rm -rf build &&
rm -rf Release &&
cmake -H. -Bbuild -DCMAKE_C_COMPILER=/usr/bin/gcc-11 -DCMAKE_CXX_COMPILER=/usr/bin/g++-11 &&
cmake --build build -- -j40
