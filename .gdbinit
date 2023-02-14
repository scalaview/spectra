add-symbol-file ./kernel/bin/kernel.elf
# add-symbol-file ./programs/apps/init/bin/init.elf
add-symbol-file ./programs/apps/shell/bin/shell.elf
target remote | qemu-system-x86_64 -hda ./bin/os.bin -S -gdb stdio -no-reboot -show-cursor
# break *0x200000
# break *0x100020
# b message_queue.c:27
# b classic_keyboard_read
b window_add_container