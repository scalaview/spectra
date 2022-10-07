
LD = x86_64-elf-ld
GCC = x86_64-elf-gcc
OBJCOPY = x86_64-elf-objcopy
ASM = nasm
OBJ_DIR = ./build
BIN_DIR = ./bin
INC_DIR = ./src/include
INCLUDES = $(addprefix -I, $(INC_DIR))
LINK_FILE = ./src/linker.ld
ASM_FLAGS = -f elf64 -g -F dwarf
STD_LIB_DIR = ../../stdlib/
STD_LIB_BIN = ../../stdlib/bin/stdlib.elf
APP_BIN = ./bin/hello.bin
APP_ELF_FILES =	./bin/start.elf
PROJECT_BUILD_PATH = ../../../build/apps

FILES =	$(STD_LIB_BIN) \
		./build/hello.o

FLAGS = -mcmodel=large -std=gnu99 -g -ffreestanding -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label $(INCLUDES) -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O0 -Iinc -save-temps=obj

dir:
	mkdir -p $(BIN_DIR)
	mkdir -p $(OBJ_DIR)

all: dir lib $(APP_BIN)

lib:
	@printf "\e[32;1mMaking dependencies\e[0m $<\n"
	make -C $(STD_LIB_DIR) clean
	make -C $(STD_LIB_DIR) all

$(APP_BIN): $(FILES)
	$(GCC) -g -T $(LINK_FILE) -o $(APP_ELF_FILES) -ffreestanding -O0 -nostdlib -fpic -g ${FILES}
	$(OBJCOPY) -O binary $(APP_ELF_FILES) $@


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
	cp $(APP_BIN) $(PROJECT_BUILD_PATH)

.PHONY: clean
