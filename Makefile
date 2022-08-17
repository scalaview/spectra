
LD = x86_64-elf-ld
GCC = x86_64-elf-gcc
ELF= elf64
BOOTFILES = ./build/boot/boot.asm.o \
			./build/boot/boot.o

FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all: ./bin/boot.bin ./bin/loader.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/loader.bin >> ./bin/os.bin
	dd if=/dev/zero bs=512 count=1 >> ./bin/os.bin

./bin/boot.bin:
	nasm -f bin ./src/boot/boot.asm -o ./bin/boot.bin

./bin/loader.bin: ./build/boot/loader.asm.o ./build/boot/loader.o
	$(LD) --build-id=none -T ./src/boot/linker.ld ./build/boot/loader.asm.o ./build/boot/loader.o -nostdlib -o ./build/boot/loader.elf
	objcopy -O binary ./build/boot/loader.elf ./bin/loader.bin

./build/boot/loader.asm.o:
	nasm -f $(ELF) -g ./src/boot/loader.asm -o ./build/boot/loader.asm.o

./build/boot/loader.o: ./src/boot/loader.c
	$(GCC) -I./src/boot $(FLAGS) -std=gnu99 -c ./src/boot/loader.c -o ./build/boot/loader.o

clean:
	rm -rf ./build/boot/boot.asm.o
	rm -rf ./build/boot/boot.o
	rm -rf ./build/boot/loader.asm.o
	rm -rf ./bin/os.bin
	rm -rf ./bin/boot.bin
	rm -rf ./bin/loader.bin