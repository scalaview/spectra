
LD = x86_64-elf-ld
GCC = x86_64-elf-gcc
ASM = nasm
OBJ_DIR = ./build
BIN_DIR = ./bin
INC_DIR = ./src/include
LIBC_DIR = ../libc
LIBC_OBJ_DIR = $(LIBC_DIR)/build
LIBC_INC_DIR = $(LIBC_DIR)/src/include
INCLUDES = $(addprefix -I, $(INC_DIR))
INCLUDES += $(addprefix -I, $(LIBC_INC_DIR))
ASM_INC_DIR = ./src/asminc
ASM_INCLUDES = $(addprefix -i, $(ASM_INC_DIR))
ASM_FLAGS = -f elf64 -g -F dwarf
LINK_FILE = ./src/linker.ld
OS_BIN_FILES =	./bin/kernel.elf

LIBC_FILES = $(LIBC_OBJ_DIR)/window/window.o

KFILES =./build/boot/multiboot_header.asm.o \
		./build/boot/stage1.asm.o \
		./build/boot/stage2.asm.o \
		./build/boot/stdlib.asm.o \
		./build/boot/reserved.asm.o \
		./build/kernel.o \
		./build/string/string.o \
		./build/memory/kmemory.asm.o \
		./build/memory/kmemory.o \
		./build/memory/pmm.o \
		./build/memory/bitmap_pmm.o \
		./build/memory/heap/bitmap_heap.o \
		./build/memory/heap/kheap.o \
		./build/printk/printk.o \
		./build/debug/assert.o \
		./build/debug/print.o \
		./build/interrupt/idt.asm.o \
		./build/interrupt/idt.o \
		./build/io/io.asm.o \
		./build/io/serial.o \
		./build/memory/paging/paging.asm.o \
		./build/memory/paging/paging.o \
		./build/disk/disk.o \
		./build/fs/path.o \
		./build/fs/file.o \
		./build/disk/stream.o \
		./build/fs/ext2/ext2.o \
		./build/gdt/gdt.asm.o \
		./build/task/tss.o \
		./build/task/tss.asm.o \
		./build/task/task.o \
		./build/task/process.o \
		./build/task/task.asm.o \
		./build/systemctl/isr80h.o \
		./build/systemctl/command/io.o \
		./build/systemctl/command/process.o \
		./build/systemctl/command/windows.o \
		./build/task/mmu/none.o \
		./build/task/command_argument.o \
		./build/drivers/keyboard/keyboard.o \
		./build/drivers/keyboard/classic.o \
		./build/drivers/vga/vesa.o \
		./build/drivers/mouse/mouse.o \
		./build/window/window_manager.o

KFILES  += $(LIBC_FILES)

FLAGS = -mcmodel=large -std=gnu99 -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label $(INCLUDES) -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc -save-temps=obj

dir:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(BIN_DIR)

all: dir $(OS_BIN_FILES)

$(OS_BIN_FILES): $(KFILES)
	@printf "\e[32;1mLinking\e[0m $@\n"
	$(LD) -nostdlib -n -T $(LINK_FILE) ${KFILES} -o $@

$(OBJ_DIR)/%.asm.o: src/%.asm
	@printf "\e[32;1mAssembling\e[0m $<\n"
	@mkdir -p $(shell dirname $@)
	@$(ASM) $(ASM_FLAGS) $(ASM_INCLUDES) $< -o $@

$(OBJ_DIR)/%.o: src/%.c
	@printf "\e[32;1mCompiling\e[0m $<\n"
	@mkdir -p $(shell dirname $@)
	$(GCC) $(INCLUDES) $(FLAGS) -c $< -o $@

$(LIBC_OBJ_DIR)/%.o: $(LIBC_DIR)/src/%.c
	@printf "\e[32;1mCompiling\e[0m $<\n"
	@mkdir -p $(shell dirname $@)
	$(GCC) $(INCLUDES) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)