add-symbol-file ./kernel/bin/kernel.elf
# add-symbol-file ./programs/apps/init/bin/init.elf
# add-symbol-file ./programs/apps/shell/bin/shell.elf
add-symbol-file ./programs/apps/desktop/bin/desktop.elf
target remote | qemu-system-x86_64 -hda ./bin/os.bin -S -gdb stdio -no-reboot -show-cursor
# break *0x200000
# break *0x100020
# b __process_load_elf_program
# b __init_program_content
# b elf64_parse_pheader
# b task_initialize
# b process_load_program
# b task.c:318
# b __process_init_elf
# b __parse_elf64_pheader
b process_load_program