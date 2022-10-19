add-symbol-file ./kernel/bin/kernel.elf
add-symbol-file ./programs/apps/init/bin/init.elf
add-symbol-file ./programs/apps/hello/bin/hello.elf
target remote | qemu-system-x86_64 -hda ./bin/os.bin -S -gdb stdio
# break *0x200000
# break *0x100020
b classic_keyboard_interrupt_handler
b keyboard_pop
b keyboard.c:78
