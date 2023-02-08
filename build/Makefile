CPPFLAGS = -std=c++20 -Os -fdata-sections -ffunction-sections -fno-threadsafe-statics -fno-omit-frame-pointer -fno-rtti -mcpu=cortex-a78 -D_GNU_SOURCE -flto -fuse-ld=lld -static-libstdc++ -fvisibility=hidden -fvisibility-inlines-hidden -Wl,--icf=all,-O3,--lto-O3,--strip-all -ffixed-x18 -Wl,--hash-style=both -fPIE -Wl,-exclude-libs,ALL -Wl,--gc-sections
CLANG = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android33-clang++
Feashelper: ../src/*.cpp
	$(CLANG) $(CPPFLAGS) $^ -o FEAShelper
	sstrip FEAShelper; true
	cp -f FEAShelper ../Module/FEAShelper

.PHONY:clean
clean:
	rm FEAShelper
