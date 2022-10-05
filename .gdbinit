add-symbol-file ./kernel/bin/kernel.elf
add-symbol-file ./programs/apps/start/bin/start.elf
target remote | qemu-system-x86_64 -hda ./bin/os.bin -S -gdb stdio
# break *0x200000
# break *0x100020
# break *0x100042
# b kernel_main
# b process_load
# b paging_initialize_pml4_table
# b task_initialize
# b process_launch
# b isr80h_command0_print
# b isr80h_handle_command
# b isr80h_wrapper
# b isr80h_handler
# b printf
# b isr80h_command0_print
# b task_switch
# b sys_print
# b main
# b paging_initialize_pml4_table
b main_entry