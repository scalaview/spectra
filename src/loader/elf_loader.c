#include "elf_loader.h"

bool elf64_valid_class(struct Elf64_Ehdr* header)
{
    return header->e_ident[EI_NIDENT_CLASS] == EI_NIDENT_64BIT;
}
