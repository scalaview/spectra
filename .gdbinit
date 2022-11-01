add-symbol-file ./kernel/bin/kernel.elf
add-symbol-file ./programs/apps/init/bin/init.elf
add-symbol-file ./programs/apps/hello/bin/hello.elf
target remote | qemu-system-x86_64 -hda ./bin/os.bin -S -gdb stdio -no-reboot -show-cursor
# break *0x200000
# break *0x100020
b ps2_mouse_interrupt_handler
b classic_keyboard_read
b mouse_initialize
# b paging.c:59