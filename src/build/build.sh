rm -rf ./Feashelper
clang++ ../feashelper/feashelper.cpp -O3 --static -s -o Feashelper
#fallback
[ ! -f $MODDIR/Feashelper ] && arm-linux-gnueabihf-c++ ../feashelper/feashelper.cpp -O3 --static -s -o ./Feashelper
[ -f ./Feashelper ] && cp -r ./Feashelper ../../Module/Feashelper