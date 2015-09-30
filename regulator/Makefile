# vim: ts=8:noet

build: src/version.h
	ino build

clean:
	ino clean

upload:
	ino upload

src/version.h:
	printf '#ifndef VERSION\n#define VERSION "%s"\n#define VERSION1 "%s"\n#define VERSION2 "%s"\n#endif\n' '$(shell git describe --tags --always --dirty=+)' '$(shell git describe --tags --always --dirty=+ | cut -d- -f1)' '$(shell git describe --tags --always --dirty=+ | cut -d- -f2-)' >$@

.PHONY: build clean upload src/version.h

