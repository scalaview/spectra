#ifndef ELF_LOADER_H
#define ELF_LOADER_H
#include <stdbool.h>
#include <stddef.h>
#include "elf.h"

struct elf_content
{
    void* base_address;
    size_t size;
    void* virtual_base_address;
    void* virtual_end_address;

    void* entry;
    struct elf_ph_content* head;
};

bool elf64_valid_class(struct Elf64_Ehdr* header);
bool elf_valid_magic(unsigned char* e_ident);
int elf64_read_header(void* prog_buffer, struct Elf64_Ehdr** header);
int elf64_parse_pheader(struct Elf64_Ehdr* elf_header, struct elf_content** out_elf);

#endif