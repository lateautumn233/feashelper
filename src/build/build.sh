rm -rf ./Feashelper
clang++ ../feashelper/feashelper.cpp -O3 --static -s -o Feashelper --target=arm64 > /dev/null 2>&1
#fallback
[ ! -f ./Feashelper ] && arm-linux-gnueabihf-c++ ../feashelper/feashelper.cpp -O3 --static -s -o Feashelper