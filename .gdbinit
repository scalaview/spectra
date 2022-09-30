add-symbol-file ./bin/kernel.elf
target remote | qemu-system-x86_64 -hda ./bin/os.bin -S -gdb stdio
# break *0x200000
# break *0x100020
# break *0x100042
b kernel_main
# b paging_initialize_pml4_table
# b task_initialize
# b process_launch
# b *0x400000
# b interrupt_handler