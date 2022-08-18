#include "disk.h"
#include "io.h"
#include <elf.h>
#include "elf_loader.h"

void bootmain()
{
    struct Elf64_Ehdr* elf_hdr = (struct Elf64_Ehdr*)0x01000000;
    disk_read_sector(5, 1, (void*)elf_hdr);
    if (!elf64_valid_class(elf_hdr))
    {
        goto out;
    }
    struct Elf64_Phdr* elf_phdr = (struct Elf64_Phdr*)((uint64_t)elf_hdr + elf_hdr->e_phoff);
    struct Elf64_Phdr* elf_end_phdr = elf_phdr + elf_hdr->e_phnum;
    if (elf_end_phdr) {}
out:
    while (1) {}
}