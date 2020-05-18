#!/bin/sh
# Copy required resources
cp Resources/converter.ico ../Release/

rm -rf ../Release/Configuration
mkdir ../Release/Configuration
cp Resources/*.yml ../Release/Configuration/

cp -r ../../CK2ToEU4/Release ../Release/CK2ToEU4
cp ../../CK2ToEU4/CK2ToEU4/Data_Files/*yml ../Release/Configuration/
cp ../../CK2ToEU4/CK2ToEU4/Data_Files/*txt ../Release/Configuration/
