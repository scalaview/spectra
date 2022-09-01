
LD = x86_64-elf-ld
GCC = x86_64-elf-gcc
NASM = nasm
SOU_DIR = ./src
OBJ_DIR = ./build
BIN_DIR = ./bin
INC_DIR = ./src/include
INCLUDES = $(addprefix -I, $(INC_DIR))
ASM_INC_DIR = ./src/asminc
ASM_INCLUDES = $(addprefix -i, $(ASM_INC_DIR))
SOU_SUB_DIR = $(sort $(dir $(wildcard $(SOU_DIR)/*/)))
OBJ_SUBDIR = $(subst $(SOU_DIR), $(OBJ_DIR),$(SOU_SUB_DIR))
NASM_FLAGS = -f elf64

LOADER_FILES = ./build/boot/stage.asm.o \
			   ./build/boot/loader.o \
			   ./build/io/io.asm.o \
			   ./build/disk/disk.o \
			   ./build/string/string.o \
			   ./build/loader/elf_loader.o

BOCHS_BASIC_IMG = bochs.img

KFILES =./build/kernel.o \
		./build/string/string.o \
		./build/memory/memory.o \
		./build/printk/printk.o \
		./build/debug/assert.o \
		./build/interrupt/idt.asm.o \
		./build/interrupt/idt.o \
		./build/io/io.asm.o

FLAGS = -fPIC -m64 -std=gnu99 -g -ffreestanding -mcmodel=large -falign-jumps -falign-functions -falign-labels -falign-loops -fstrength-reduce -fomit-frame-pointer -finline-functions -Wno-unused-function -fno-builtin -Werror -Wno-unused-label $(INCLUDES) -Wno-cpp -Wno-unused-parameter -nostdlib -nostartfiles -nodefaultlibs -Wall -O2 -Iinc -save-temps=obj

dir:
	mkdir -p $(BIN_DIR) $(OBJ_SUBDIR)

all: dir ./bin/boot.bin ./bin/loader.bin ./bin/kernel.elf
	dd if=/dev/zero bs=512 count=5 >> ./bin/os.bin
	dd if=./bin/boot.bin of=./bin/os.bin bs=512 count=1 conv=notrunc
	dd if=./bin/loader.bin of=./bin/os.bin bs=512 count=4 seek=1 conv=notrunc
	dd if=./bin/kernel.elf of=./bin/os.bin bs=512 count=20000 seek=5 conv=notrunc
	cp $(BOCHS_BASIC_IMG) ./bin/os_bochs.bin
	dd if=./bin/boot.bin of=./bin/os_bochs.bin bs=512 count=1 conv=notrunc
	dd if=./bin/loader.bin of=./bin/os_bochs.bin bs=512 count=4 seek=1 conv=notrunc
	dd if=./bin/kernel.elf of=./bin/os_bochs.bin bs=512 count=20000 seek=5 conv=notrunc

./bin/boot.bin: ./src/boot/boot.asm
	$(NASM) -f bin ./src/boot/boot.asm -o ./bin/boot.bin

./bin/loader.bin: $(LOADER_FILES)
	$(LD) --build-id=none -T ./src/boot/linker.ld $(LOADER_FILES) -nostdlib -o ./build/boot/loader.elf
	objcopy -O binary ./build/boot/loader.elf ./bin/loader.bin

./bin/kernel.elf: $(KFILES)
	$(LD) -nostdlib -static -T ./src/linker.ld ${KFILES} -o ./bin/kernel.elf
	objcopy -O binary ./bin/kernel.elf ./bin/kernel.bin

./build/boot/stage.asm.o: ./src/boot/stage.asm
	$(NASM) $(NASM_FLAGS) $(ASM_INCLUDES) -g ./src/boot/stage.asm -o ./build/boot/stage.asm.o

./build/boot/loader.o: ./src/boot/loader.c
	$(GCC) -I./src/boot $(FLAGS) -c ./src/boot/loader.c -o ./build/boot/loader.o

./build/io/io.asm.o: ./src/io/io.asm
	$(NASM) $(NASM_FLAGS) $(ASM_INCLUDES) -g ./src/io/io.asm -o ./build/io/io.asm.o

./build/disk/disk.o: ./src/disk/disk.c
	$(GCC) $(INCLUDES) $(FLAGS) -c ./src/disk/disk.c -o ./build/disk/disk.o

./build/string/string.o: ./src/string/string.c
	$(GCC) $(INCLUDES) $(FLAGS) -c ./src/string/string.c -o ./build/string/string.o

./build/memory/memory.o: ./src/memory/memory.c
	$(GCC) $(INCLUDES) $(FLAGS) -c ./src/memory/memory.c -o ./build/memory/memory.o

./build/printk/printk.o: ./src/printk/printk.c
	$(GCC) $(INCLUDES) $(FLAGS) -c ./src/printk/printk.c -o ./build/printk/printk.o

./build/debug/assert.o: ./src/debug/assert.c
	$(GCC) $(INCLUDES) $(FLAGS) -c ./src/debug/assert.c -o ./build/debug/assert.o

./build/loader/elf_loader.o: ./src/loader/elf_loader.c
	$(GCC) $(INCLUDES) $(FLAGS) -c ./src/loader/elf_loader.c -o ./build/loader/elf_loader.o

./build/kernel.asm.o: ./src/kernel.asm
	$(NASM) $(NASM_FLAGS) $(ASM_INCLUDES) -g ./src/kernel.asm -o ./build/kernel.asm.o

./build/kernel.o: ./src/kernel.c
	$(GCC) $(INCLUDES) $(FLAGS) -c ./src/kernel.c -o ./build/kernel.o

./build/interrupt/idt.asm.o: ./src/interrupt/idt.asm
	$(NASM) $(NASM_FLAGS) $(ASM_INCLUDES) -g ./src/interrupt/idt.asm -o ./build/interrupt/idt.asm.o

./build/interrupt/idt.o: ./src/interrupt/idt.c
	$(GCC) $(INCLUDES) $(FLAGS) -c ./src/interrupt/idt.c -o ./build/interrupt/idt.o

clean:
	rm -rf $(OBJ_DIR)
	rm -rf $(BIN_DIR)