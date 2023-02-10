CPPFLAGS = -Wall -std=c++20 -Os -flto -stdlib=libc++ -mcpu=cortex-a78 -fno-rtti -fvisibility=hidden -fvisibility-inlines-hidden -fno-threadsafe-statics -fno-omit-frame-pointer -fPIE -static-libstdc++ -fuse-ld=lld -Wl,-O3,--lto-O3,--gc-sections,--icf=all,-s,-x
CLANG = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android33-clang++
Feashelper: ../src/*.cpp
	$(CLANG) $(CPPFLAGS) $^ -o FEAShelper
	sstrip FEAShelper; true
	cp -f FEAShelper ../Module/FEAShelper

.PHONY:clean
clean:
	rm -f FEAShelper
