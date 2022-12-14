#ifndef ELF_LOADER_H
#define ELF_LOADER_H
#include <stdbool.h>

#include "elf.h"

bool elf64_valid_class(struct Elf64_Ehdr* header);
bool elf_valid_magic(unsigned char* e_ident);

#endif