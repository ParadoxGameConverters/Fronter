#!/bin/bash
cd Fronter &&
rm -rf build &&
rm -rf ../Release &&
cmake -H. -Bbuild -DCMAKE_C_COMPILER=/usr/bin/gcc-10 -DCMAKE_CXX_COMPILER=/usr/bin/g++-10 &&
cmake --build build -- -j3 &&
cd ..
