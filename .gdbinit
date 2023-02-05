add-symbol-file ./kernel/bin/kernel.elf
# add-symbol-file ./programs/apps/init/bin/init.elf
add-symbol-file ./programs/apps/desktop/bin/desktop.elf
target remote | qemu-system-x86_64 -hda ./bin/os.bin -S -gdb stdio -no-reboot -show-cursor
# break *0x200000
# break *0x100020
b kernel_init_vesa
b main
b gfx_puts
# b isr80h_command9_create_window_content