#!/bin/sh
# Copy required resources
cp Resources/converter.ico ../Release/

rm -rf ../Release/Configuration
mkdir ../Release/Configuration
cp Resources/*.yml ../Release/Configuration/
