rm -rf ./Feashelper
# clang++ ../feashelper/addconfig.cpp ../feashelper/Androidutils_feas.cpp ../feashelper/Androidutils.cpp ../feashelper/feashelper.cpp -std=c++20 -O3 --static -flto -mcpu=cortex-a78 -fuse-ld=lld -Wl,-s,-x -o Feashelper
clang++ ../feashelper/Androidutils_feas.cpp ../feashelper/Androidutils.cpp ../feashelper/feashelper.cpp -std=c++20 -O3 --static -flto -mcpu=cortex-a78 -fuse-ld=lld -Wl,-s,-x -o Feashelper