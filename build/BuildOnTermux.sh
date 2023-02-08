aarch64-linux-android-clang++ --std=c++20 -Os -fdata-sections -ffunction-sections -fno-threadsafe-statics -fno-omit-frame-pointer -fno-rtti -mcpu=cortex-a78 -D_GNU_SOURCE -flto -fuse-ld=lld -static-libstdc++ -fvisibility=hidden -fvisibility-inlines-hidden -Wl,--icf=all,-O3,--lto-O3,--strip-all -ffixed-x18 -Wl,--hash-style=both -fPIE -Wl,-exclude-libs,ALL -Wl,--gc-sections ../src/*.cpp -o FEAShelper
sstrip ./FEAShelper; true
cp -f FEAShelper ../Module/FEAShelper