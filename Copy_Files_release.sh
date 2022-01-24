#!/bin/sh
# Copy required resources
cp Resources/converter.ico ../Release/
cp Resources/GeneralFAQ-READ.ME.FIRST.txt ../Release/

rm -rf ../Release/Configuration
mkdir ../Release/Configuration
cp Resources/*.yml ../Release/Configuration/
