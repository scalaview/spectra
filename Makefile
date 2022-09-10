
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
OS_BIN_DISK = ./bin/disk

BOCHS_BASIC_IMG = bochs.img

OS_BIN_FILES =	./bin/kernel.elf

KFILES =./build/boot/multiboot_header.asm.o \
		./build/boot/stage1.asm.o \
		./build/boot/stage2.asm.o \
		./build/boot/stdlib.asm.o \
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
	@mkdir -p $(OS_BIN_DISK)/boot/grub
	@cp ./bin/kernel.elf $(OS_BIN_DISK)/boot/kernel.elf
	@cp ./src/grub.cfg $(OS_BIN_DISK)/boot/grub
	@grub-mkrescue -o $(OS_BIN) $(OS_BIN_DISK)
	@make boch_img

boch_img:
	cp $(BOCHS_BASIC_IMG) $(OS_BOCHS_BIN)
	dd if=$(OS_BIN) of=$(OS_BOCHS_BIN) bs=512 count=20000 conv=notrunc

./bin/kernel.elf: $(KFILES)
	$(LD) -nostdlib -n -T ./src/linker.ld ${KFILES} -o ./bin/kernel.elf

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