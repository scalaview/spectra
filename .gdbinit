add-symbol-file ./kernel/bin/kernel.elf
add-symbol-file ./programs/apps/init/bin/init.elf
target remote | qemu-system-x86_64 -hda ./bin/os.bin -S -gdb stdio
# break *0x200000
# break *0x100020
# break *0x100042
# b kernel_main
# b isr80h_command5_execve
# b process_execve
# b task_launch
# b task_initialize
# b task_switch
# b isr80h_wrapper
# b sys_execve
b isr80h_command4_fork
b task_clone