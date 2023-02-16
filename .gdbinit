add-symbol-file ./kernel/bin/kernel.elf
# add-symbol-file ./programs/apps/init/bin/init.elf
add-symbol-file ./programs/apps/desktop/bin/desktop.elf
target remote | qemu-system-x86_64 -hda ./bin/os.bin -S -gdb stdio -no-reboot -show-cursor
# break *0x200000
# break *0x100020
# b message_queue.c:27
# b classic_keyboard_read
# b create_gui_window
# b isr80h_command9_create_window_content
# b window_refresh
b create_window_content
b isr80h_command9_create_window_content