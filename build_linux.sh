#!/bin/bash
cd Fronter &&
rm -rf build &&
rm -rf ../Release &&
cmake -H. -Bbuild -DCMAKE_C_COMPILER=/usr/bin/gcc-11 -DCMAKE_CXX_COMPILER=/usr/bin/g++-11 &&
cmake --build build -- -j40 &&
cd ..
