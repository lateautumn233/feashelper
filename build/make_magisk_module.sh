#!/usr/sbin/bash

# $1 : module name
if [ ! -f Feashelper ]; then
    exit 1
fi
rm -r *.zip
cp -f ./Feashelper ../Module/Feashelper
zip -9 -r -j -J $1 "../Module"