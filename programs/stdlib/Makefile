
LD = x86_64-elf-ld
GCC = x86_64-elf-gcc
ASM = nasm
AR = x86_64-elf-ar
OBJ_DIR = ./build
BIN_DIR = ./bin
INC_DIR = ./src/include
LIBC_DIR = ../../libc
LIBC_OBJ_DIR = $(LIBC_DIR)/build
LIBC_INC_DIR = $(LIBC_DIR)/src/include

INCLUDES = $(addprefix -I, $(INC_DIR))
INCLUDES += $(addprefix -I, $(LIBC_INC_DIR))

ASM_FLAGS = -f elf64 -g -F dwarf

STD_OBJ = ./bin/stdlib.elf
PROJECT_BUILD_PATH = ../../build/lib

LIBC_FILES = $(LIBC_OBJ_DIR)/window/window.o

FILES =	./build/entry.asm.o \
		./build/memory.o \
		./build/stdio.o \
		./build/string.o \
		./build/syscall.asm.o \
		./build/unistd.o \
		./build/process.o \
		./build/vesa.o \
		./build/messages.o \
		./build/gui/button.o \
		./build/gui/label.o \
		./build/gui/gui.o \
		./build/assets/img/tga.o \
		./build/assets/img/parser.o

FILES  += $(LIBC_FILES)

FLAGS = -mcmodel=large -std=gnu99 -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label $(INCLUDES) -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc -save-temps=obj

dir:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)

all: dir $(STD_OBJ)

$(STD_OBJ): $(FILES)
	$(LD) -m elf_x86_64 -relocatable ${FILES} -o $(STD_OBJ)


$(OBJ_DIR)/%.asm.o: src/%.asm
	@printf "\e[32;1mAssembling\e[0m $<\n"
	@mkdir -p $(shell dirname $@)
	@$(ASM) $(ASM_FLAGS) $< -o $@

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

install:
	cp $(STD_OBJ) $(PROJECT_BUILD_PATH)

.PHONY: clean all install