#include "elf_loader.h"
#include "string.h"
#include "file.h"
#include "elf.h"
#include "heap/kheap.h"
#include "status.h"

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

bool elf_executable(Elf64_Ehdr* elf64_header)
{
    return elf64_header->e_type == ET_EXEC;
}

int elf64_read_header(FILE* fd, Elf64_Ehdr header)
{
    int res = 0;

    Elf64_Ehdr* elf64_header = (Elf64_Ehdr*)kzalloc(sizeof(Elf64_Ehdr));
    if(!elf64_header)
    {
        res = -ENOMEM;
        goto out;
    }

    fread(elf64_header, sizeof(Elf64_Ehdr), 1, fd);

    if(!elf_valid_magic(elf64_header) || !elf64_valid_class(elf64_header))
    {
        res = -EELFMAGIC;
        goto out;
    }

    if(!elf_executable(elf64_header))
    {
        res = -ENOEXEC;
        goto out;
    }


    header = *elf64_header;
    return res;
out:
    free(elf64_header);
    return res;
}