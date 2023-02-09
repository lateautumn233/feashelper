aarch64-linux-android-clang++ -Wall -std=c++20 -Os -flto -static -stdlib=libc++ -march=sapphirerapids -fno-rtti -fvisibility=hidden -fvisibility-inlines-hidden -fuse-ld=lld -Wl,-O3,--lto-O3,--gc-sections,--icf=all,-s,-x ../src/*.cpp -o FEAShelper
sstrip ./FEAShelper; true
cp -f FEAShelper ../Module/FEAShelper
