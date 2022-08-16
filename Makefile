
LD = x86_64-elf-ld
GCC = x86_64-elf-gcc
BOOTFILES = ./build/boot/boot.asm.o \
			./build/boot/boot.o

FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all: ./bin/boot.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=/dev/zero bs=1024 count=1 >> ./bin/os.bin

./bin/boot.bin: ./build/boot/boot.asm.o ./build/boot/boot.o
	$(LD) -g -relocatable $(BOOTFILES) -o ./build/boot.o
	$(GCC) -T ./src/boot/linker.ld -o ./bin/boot.bin -ffreestanding -O0 -nostdlib ./build/boot.o

./build/boot/boot.asm.o: ./src/boot/boot.asm
	nasm -f elf -g ./src/boot/boot.asm -o ./build/boot/boot.asm.o

./build/boot/boot.o: ./src/boot/boot.c
	$(GCC) -I./src/boot $(FLAGS) -std=gnu99 -c ./src/boot/boot.c -o ./build/boot/boot.o

clean:
	rm -rf ./bin/os.bin
	rm -rf ./bin/boot.bin