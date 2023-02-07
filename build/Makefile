CPPFLAGS = -std=c++20 -Os -ffunction-sections -fdata-sections -mcpu=cortex-a78 -flto -fuse-ld=lld -static-libstdc++ -Wl,-s,-x
CLANG = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/aarch64-linux-android33-clang++
Feashelper: ../src/*.cpp
	$(CLANG) $(CPPFLAGS) $^ -o FEAShelper
	sstrip FEAShelper; true
	cp -f FEAShelper ../Module/FEAShelper

.PHONY:clean
clean:
	rm FEAShelper
