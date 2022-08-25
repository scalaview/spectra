#include <stdint.h>

#include "disk.h"
#include "elf.h"
#include "elf_loader.h"
#include "config.h"


static void readseg(uint64_t v_addr, uint64_t f_offset, uint64_t p_filesz) {

    uint64_t sector_offset = p_filesz % SECTOR_SIZE;
    uint64_t total_sectors = sector_offset > 0 ? ((p_filesz + SECTOR_SIZE) / SECTOR_SIZE) : p_filesz / SECTOR_SIZE;

    // translate from bytes to sectors; kernel starts at sector 5
    uint32_t sector_start_no = (f_offset / SECTOR_SIZE) + 5;

    disk_read_sector(sector_start_no, total_sectors, (void*)v_addr);
}

void bootmain()
{
    struct Elf64_Ehdr* elf_hdr = (struct Elf64_Ehdr*)0x00100000;
    disk_read_sector(5, 1, (void*)elf_hdr);
    if (!elf_valid_magic(elf_hdr->e_ident) || !elf64_valid_class(elf_hdr))
    {
        goto out;
    }
    struct Elf64_Phdr* elf_phdr = (struct Elf64_Phdr*)((uint64_t)elf_hdr + elf_hdr->e_phoff);
    struct Elf64_Phdr* end_elf_phdr = elf_phdr + elf_hdr->e_phnum;
    for (; elf_phdr < end_elf_phdr; elf_phdr++)
    {
        readseg(elf_phdr->p_vaddr & 0xFFFFFF, elf_phdr->p_offset, elf_phdr->p_filesz);
    }
    ((void (*)(void))(elf_hdr->e_entry & 0xFFFFFF))();
out:
    while (1) {}
}