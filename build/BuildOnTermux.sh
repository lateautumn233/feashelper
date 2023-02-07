aarch64-linux-android-clang++ -std=c++20 ../src/*.cpp -flto -fuse-ld=lld -Os -ffunction-sections -Wl,-s,-x -static-libstdc++ -o FEAShelper
sstrip ./FEAShelper; true
cp -f FEAShelper ../Module/FEAShelper