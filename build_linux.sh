#!/bin/bash
export CC=/usr/bin/gcc-11 &&
export CXX=/usr/bin/g++-11 &&
rm -rf build &&
rm -rf Release &&
cmake -H. --preset x64-coverage-linux -DCMAKE_C_COMPILER=/usr/bin/gcc-11 -DCMAKE_CXX_COMPILER=/usr/bin/g++-11 &&
cmake --build --preset build-x64-coverage-linux --target ConverterFrontend -- -j40
