rm -rf ./Feashelper
# clang++ ../src/addconfig.cpp ../src/Androidutils_feas.cpp ../src/Androidutils.cpp ../src/feashelper.cpp ../src/S3profile.cpp -std=c++20 -O3 --static -flto -mcpu=cortex-a78 -fuse-ld=lld -Wl,-s,-x -o Feashelper
clang++ ../src/Androidutils_feas.cpp ../src/Androidutils.cpp ../src/feashelper.cpp ../src/S3profile.cpp -std=c++20 -O3 --static -flto -mcpu=cortex-a78 -fuse-ld=lld -Wl,-s,-x -o Feashelper
