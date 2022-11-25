add-symbol-file ./kernel/bin/kernel.elf
add-symbol-file ./programs/apps/init/bin/init.elf
add-symbol-file ./programs/apps/hello/bin/hello.elf
target remote | qemu-system-x86_64 -hda ./bin/os.bin -S -gdb stdio -no-reboot -show-cursor
# break *0x200000
# break *0x100020
# b test_draw
# b draw_cursor
b printk.c:170
# b draw_background

# b paging.c:59