CPPFLAGS = -O3 -march=arm64 -mcpu=cortex-a78
LDFLAGS = -flto -O3 -fuse-ld=lld --static -Wl,-s,-x
CLANG = $(NDK)/toolchains/llvm/prebuilt/linux-x86_64/bin/armv7a-linux-androideabi33-clang++
Feashelper: feashelper.o Androidutils.o Androidutils_feas.o S3profile.o
	$(CLANG) $^ $(LDFLAGS) -o Feashelper
	sstrip Feashelper; true
	cp -f Feashelper ../Module/Feashelper

feashelper.o:
	$(CLANG) -c ../src/feashelper.cpp $(CPPFLAGS)

Androidutils.o:
	$(CLANG) -c ../src/Androidutils.cpp $(CPPFLAGS)

Androidutils_feas.o:
	$(CLANG) -c ../src/Androidutils_feas.cpp $(CPPFLAGS)

S3profile.o:
	$(CLANG) -c ../src/S3profile.cpp $(CPPFLAGS)

.PHONY:clean
clean:
	rm *.o