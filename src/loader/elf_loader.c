#include "elf_loader.h"
#include "string.h"

const char ELFMAGIC[4] = { 0x7F, 'E', 'L', 'F' };  // "\177ELF";

bool elf64_valid_class(struct Elf64_Ehdr* header)
{
    return header->e_ident[EI_NIDENT_CLASS] == EI_NIDENT_64BIT;
}

bool elf32_valid_class(struct Elf64_Ehdr* header)
{
    return header->e_ident[EI_NIDENT_CLASS] == EI_NIDENT_32BIT;
}

bool elf_valid_magic(unsigned char* e_ident)
{
    return strncmp((char*)e_ident, ELFMAGIC, sizeof(ELFMAGIC)) == 0;
}