aarch64-linux-android-clang++ -Wall -std=c++20 -Os -flto -stdlib=libc++ -mcpu=cortex-a78 -fno-rtti -fvisibility=hidden -static-libstdc++ -fuse-ld=lld -Wl,-O3,--lto-O3,--gc-sections,--icf=all,-s,-x ../src/*.cpp -o FEAShelper
strip ./FEAShelper; true
cp -f FEAShelper ../Module/FEAShelper