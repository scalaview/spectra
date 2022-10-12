
LD = x86_64-elf-ld
GCC = x86_64-elf-gcc
ASM = nasm
AR = x86_64-elf-ar
OBJ_DIR = ./build
BIN_DIR = ./bin
INC_DIR = ./src/include
INCLUDES = $(addprefix -I, $(INC_DIR))
ASM_FLAGS = -f elf64 -g -F dwarf

STD_OBJ = ./bin/stdlib.elf
PROJECT_BUILD_PATH = ../../build/lib

FILES =	./build/entry.asm.o \
		./build/memory.o \
		./build/stdio.o \
		./build/string.o \
		./build/syscall.asm.o \
		./build/unistd.o \
		./build/process.o

FLAGS = -mcmodel=large -std=gnu99 -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label $(INCLUDES) -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc -save-temps=obj

dir:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)

all: dir $(STD_OBJ)

$(STD_OBJ): $(FILES)
	$(LD) -m elf_x86_64 -relocatable ${FILES} -o $(STD_OBJ)


build/%.asm.o: src/%.asm
	@printf "\e[32;1mAssembling\e[0m $<\n"
	@mkdir -p $(shell dirname $@)
	@$(ASM) $(ASM_FLAGS) $< -o $@

build/%.o: src/%.c
	@printf "\e[32;1mCompiling\e[0m $<\n"
	@mkdir -p $(shell dirname $@)
	$(GCC) $(INCLUDES) $(FLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)

install:
	cp $(STD_OBJ) $(PROJECT_BUILD_PATH)

.PHONY: clean all install