INCLUDE_HDR = -I bin
INCLUDE_LBR = -L bin

all: gtrm.c
	mkdir -p bin
	cp *.h bin
	./jobsignal/build/autobuild.sh
	armv6-axis-linux-gnueabi-gcc -o gtrm ${INCLUDE_HDR} ${INCLUDE_LBR} gtrm.c -lcg -lpthread -lm -ljobsignaler -lrt
	mv gtrm bin/.

clean:
	rm -rf bin
