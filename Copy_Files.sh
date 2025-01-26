#!/bin/sh
# Copy required resources
cp Fronter/Resources/converter.ico Release/
cp Fronter/Resources/GeneralFAQ-READ.ME.FIRST.txt Release/

rm -rf Release/Configuration
mkdir Release/Configuration
cp Fronter/Resources/*.yml Release/Configuration/
