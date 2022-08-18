
LD = x86_64-elf-ld
GCC = x86_64-elf-gcc
ELF= elf64

LOADER_FILES = ./build/boot/loader.asm.o \
			   ./build/boot/loader.o \
			   ./build/io/io.asm.o \
			   ./build/disk/disk.o

INCLUDES = -I./src/include

FLAGS = -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label $(INCLUDES) -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc

all: ./bin/boot.bin ./bin/loader.bin ./bin/test.bin
	dd if=./bin/boot.bin >> ./bin/os.bin
	dd if=./bin/loader.bin >> ./bin/os.bin
	dd if=/dev/zero bs=512 count=10 >> ./bin/os.bin
	dd if=./bin/test.bin of=./bin/os.bin bs=512 count=1 seek=5 conv=notrunc

./bin/boot.bin: ./src/boot/boot.asm
	nasm -f bin ./src/boot/boot.asm -o ./bin/boot.bin

./bin/loader.bin: $(LOADER_FILES)
	$(LD) --build-id=none -T ./src/boot/linker.ld $(LOADER_FILES) -nostdlib -o ./build/boot/loader.elf
	objcopy -O binary ./build/boot/loader.elf ./bin/loader.bin

./build/boot/loader.asm.o: ./src/boot/loader.asm
	nasm -f $(ELF) -g ./src/boot/loader.asm -o ./build/boot/loader.asm.o

./build/boot/loader.o: ./src/boot/loader.c
	$(GCC) -I./src/boot $(FLAGS) -std=gnu99 -c ./src/boot/loader.c -o ./build/boot/loader.o

./build/io/io.asm.o: ./src/io/io.asm
	nasm -f $(ELF) -g ./src/io/io.asm -o ./build/io/io.asm.o

./build/disk/disk.o: ./src/disk/disk.c
	$(GCC) $(INCLUDES) -I./src/include/disk $(FLAGS) -std=gnu99 -c ./src/disk/disk.c -o ./build/disk/disk.o

./bin/test.bin: ./build/test.asm.o
	objcopy -O binary ./build/test.asm.o ./bin/test.bin

./build/test.asm.o: ./src/test.asm
	nasm -f $(ELF) -g ./src/test.asm -o ./build/test.asm.o

clean:
	rm -rf ./build/boot/boot.asm.o
	rm -rf ./build/boot/boot.o
	rm -rf ./build/boot/loader.asm.o
	rm -rf ./bin/os.bin
	rm -rf ./bin/boot.bin
	rm -rf ./bin/loader.bin