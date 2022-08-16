
LD = x86_64-elf-ld
GCC = x86_64-elf-gcc
BOOTFILES = ./build/boot/boot.asm.o \
			./build/boot/boot.o

FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all: ./bin/boot.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=/dev/zero bs=512 count=1 >> ./bin/os.bin

./bin/boot.bin: ./build/boot/boot.asm.o ./build/boot/boot.o
	$(LD) --build-id=none -T ./src/boot/linker.ld ./build/boot/boot.asm.o ./build/boot/boot.o -O0 -nostdlib -o ./build/boot/boot.elf
	objcopy -O binary ./build/boot/boot.elf ./bin/boot.bin

./build/boot/boot.asm.o: ./src/boot/boot.asm
	nasm -f elf64 -g ./src/boot/boot.asm -o ./build/boot/boot.asm.o

./build/boot/boot.o: ./src/boot/boot.c
	$(GCC) -I./src/boot $(FLAGS) -std=gnu99 -c ./src/boot/boot.c -o ./build/boot/boot.o

clean:
	rm -rf ./build/boot/boot.asm.o
	rm -rf ./build/boot/boot.o
	rm -rf ./bin/os.bin
	rm -rf ./bin/boot.bin