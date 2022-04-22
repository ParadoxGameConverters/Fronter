#! /bin/bash

set -eu

if [ "$BUILD_CONVERTERFRONTEND" = true ]
then
  printf "\nBuilding ConverterFrontend...\n"

  cd $GITHUB_WORKSPACE
  if [ "$RUNNER_OS" = "Windows" ]
  then
    "$MSBUILD_PATH" -m -p:Configuration=Release -p:Platform="x64" Fronter/Fronter.vcxproj
    mv Fronter/Release Release
  else
    sudo apt install libcurl4-openssl-dev
    sudo apt-key adv --fetch-keys https://repos.codelite.org/CodeLite.asc
    sudo apt-add-repository 'deb https://repos.codelite.org/wx3.1.5/ubuntu/ focal universe'
    sudo add-apt-repository ppa:ubuntu-toolchain-r/test
    sudo apt update
    sudo apt install gcc-11 g++-11
    sudo apt install libwxbase3.1-0-unofficial libwxbase3.1unofficial-dev libwxgtk3.1-0-unofficial libwxgtk3.1unofficial-dev wx3.1-headers wx-common
    # Link gcc-11 and g++-11 to their standard commands
    sudo ln -s /usr/bin/gcc-11 /usr/local/bin/gcc
    sudo ln -s /usr/bin/g++-11 /usr/local/bin/g++
    # Export CC and CXX to tell cmake which compiler to use
    export CC=/usr/bin/gcc-11
    export CXX=/usr/bin/g++-11
    # Check versions of gcc, g++ and cmake
    gcc -v && g++ -v && cmake --version

    rm -rf Release/
    cd Fronter
    ./build_linux.sh
    mv Release ../
  fi

  printf "\n✔ Successfully built ConverterFrontend.\n"
fi


if [ "$BUILD_UPDATER" = true ]
then
  printf "\nBuilding updater...\n"

  cd $GITHUB_WORKSPACE
  cd Fronter/Updater
  pip3 install -r requirements.txt
  pyinstaller --icon=updater.ico updater.py
  mkdir ../../Release/
  mv dist/updater/ ../../Release/Updater/

  printf "\n✔ Successfully built updater.\n"
fi
