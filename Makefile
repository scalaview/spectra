
LD = x86_64-elf-ld
GCC = x86_64-elf-gcc
ASM = nasm
OBJ_DIR = ./build
BIN_DIR = ./bin
INC_DIR = ./src/include
INCLUDES = $(addprefix -I, $(INC_DIR))
ASM_INC_DIR = ./src/asminc
ASM_INCLUDES = $(addprefix -i, $(ASM_INC_DIR))
ASM_FLAGS = -f elf64 -g -F dwarf
OS_BIN = ./bin/os.bin
OS_BOCHS_BIN =./bin/os_bochs.bin

LOADER_FILES = ./build/boot/stage.asm.o \
			   ./build/boot/loader.o \
			   ./build/io/io.asm.o \
			   ./build/disk/disk.o \
			   ./build/string/string.o \
			   ./build/loader/elf_loader.o

BOCHS_BASIC_IMG = bochs.img

OS_BIN_FILES =  ./bin/boot.bin \
				./bin/loader.bin \
				./bin/kernel.elf

KFILES =./build/kernel.asm.o \
		./build/kernel.o \
		./build/string/string.o \
		./build/memory/kmemory.o \
		./build/memory/pmm.o \
		./build/memory/bitmap_pmm.o \
		./build/memory/heap/bitmap_heap.o \
		./build/memory/heap/kheap.o \
		./build/printk/printk.o \
		./build/debug/assert.o \
		./build/interrupt/idt.asm.o \
		./build/interrupt/idt.o \
		./build/io/io.asm.o \
		./build/memory/paging/paging.asm.o \
		./build/memory/paging/paging.o

FLAGS = -mcmodel=large -std=gnu99 -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label $(INCLUDES) -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc -save-temps=obj

dir:
	mkdir -p $(BIN_DIR)

all: dir $(OS_BIN_FILES)
	dd if=/dev/zero bs=512 count=5 >> $(OS_BIN)
	dd if=./bin/boot.bin of=$(OS_BIN) bs=512 count=1 conv=notrunc
	dd if=./bin/loader.bin of=$(OS_BIN) bs=512 count=4 seek=1 conv=notrunc
	dd if=./bin/kernel.elf of=$(OS_BIN) bs=512 count=20000 seek=5 conv=notrunc
	cp $(BOCHS_BASIC_IMG) $(OS_BOCHS_BIN)
	dd if=./bin/boot.bin of=$(OS_BOCHS_BIN) bs=512 count=1 conv=notrunc
	dd if=./bin/loader.bin of=$(OS_BOCHS_BIN) bs=512 count=4 seek=1 conv=notrunc
	dd if=./bin/kernel.elf of=$(OS_BOCHS_BIN) bs=512 count=20000 seek=5 conv=notrunc

./bin/boot.bin: ./src/boot/boot.asm
	$(ASM) -f bin ./src/boot/boot.asm -o ./bin/boot.bin

./bin/loader.bin: $(LOADER_FILES)
	$(LD) --build-id=none -T ./src/boot/linker.ld $(LOADER_FILES) -nostdlib -o ./build/boot/loader.elf
	objcopy -O binary ./build/boot/loader.elf ./bin/loader.bin

./bin/kernel.elf: $(KFILES)
	$(LD) --build-id=none -O0 -nostdlib -T ./src/linker.ld ${KFILES} -o ./bin/kernel.elf
	objcopy -O binary ./bin/kernel.elf ./bin/kernel.bin

build/%.asm.o: src/%.asm
	@printf "\e[32;1mAssembling\e[0m $<\n"
	@mkdir -p $(shell dirname $@)
	@$(ASM) $(ASM_FLAGS) $(ASM_INCLUDES) $< -o $@

build/%.o: src/%.c
	@printf "\e[32;1mCompiling\e[0m $<\n"
	@mkdir -p $(shell dirname $@)
	$(GCC) $(INCLUDES) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)