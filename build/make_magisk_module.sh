#!/usr/sbin/bash

# $1 : module name
if [ ! -f ./Feashelper ]; then
    exit 1
fi
cp -f ./Feashelper ../Module/Feashelper
zip -9 -r -j $1 "../Module"