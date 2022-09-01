add-symbol-file ./build/boot/loader.elf 0x7e00
add-symbol-file ./bin/kernel.elf 0x200000
target remote | qemu-system-x86_64 -hda ./bin/os.bin -S -gdb stdio
break *0x7e00