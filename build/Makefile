CPPFLAGS = -std=c++20 -O3 -ffunction-sections  -mcpu=cortex-a78 -flto -fuse-ld=lld --static -Wl,-s,-x
CLANG = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android33-clang++
Feashelper: ../src/*.cpp
	$(CLANG) $(CPPFLAGS) $^ -o Feashelper
	sstrip Feashelper; true
	cp -f Feashelper ../Module/Feashelper

.PHONY:clean
clean:
	rm Feashelper
