#include "elf_loader.h"
#include "string.h"
#include "heap/kheap.h"
#include "status.h"
#include "debug.h"
#include "kmemory.h"

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

bool elf_executable(struct Elf64_Ehdr* elf64_header)
{
    return elf64_header->e_type == ET_EXEC;
}

int elf64_valid(struct Elf64_Ehdr* elf64_header)
{
    if (!elf_valid_magic(elf64_header->e_ident))
    {
        debug_printf("invalid elf file.\n");
        return -EELFMAGIC;
    }

    if (!elf64_valid_class(elf64_header))
    {
        debug_printf("unsupported 64bit elf file.\n");
        return -EELFMAGIC;
    }

    if (!elf_executable(elf64_header))
    {
        debug_printf("unsupported type elf file unexecutable.\n");
        return -ENOEXEC;
    }
    return SUCCESS;
}

void elf64_debug_header(struct Elf64_Ehdr* header)
{
    debug_printf("elf64 header Type e_type: 0x%X\n", header->e_type);
    debug_printf("elf64 header Machine e_machine: 0x%X\n", header->e_machine);
    debug_printf("elf64 header Version e_version: 0x%X\n", header->e_version);
    debug_printf("elf64 header Entry point address e_entry: 0x%X\n", header->e_entry);
    debug_printf("elf64 header Start of program headers e_phoff: 0x%X\n", header->e_phoff);
    debug_printf("elf64 header Start of section headers e_shoff: 0x%X\n", header->e_shoff);
    debug_printf("elf64 header Flags e_flags: 0x%X\n", header->e_flags);
    debug_printf("elf64 header Size of this header e_ehsize: %u\n", header->e_ehsize);
    debug_printf("elf64 header Size of program headers e_phentsize: %u\n", header->e_phentsize);
    debug_printf("elf64 header Number of program headers e_phnum: %d\n", header->e_phnum);
    debug_printf("elf64 header Size of section headers e_shentsize: %u\n", header->e_shentsize);
    debug_printf("elf64 header Number of section headers e_shnum: %d\n", header->e_shnum);
    debug_printf("elf64 header Section header string table index e_shstrndx: %d\n", header->e_shstrndx);
}

void elf64_debug_prog_header(struct Elf64_Phdr* prog_header)
{
    debug_printf("elf64 program header p_type;: 0x%X\n", prog_header->p_type);
    debug_printf("elf64 program header p_flags: 0x%X\n", prog_header->p_flags);
    debug_printf("elf64 program header p_offse: 0x%X\n", prog_header->p_offset);
    debug_printf("elf64 program header p_vaddr: 0x%X\n", prog_header->p_vaddr);
    debug_printf("elf64 program header p_paddr: 0x%X\n", prog_header->p_paddr);
    debug_printf("elf64 program header p_files: %d\n", prog_header->p_filesz);
    debug_printf("elf64 program header p_memsz: %d\n", prog_header->p_memsz);
    debug_printf("elf64 program header p_align: 0x%X\n", prog_header->p_align);
}

int elf64_read_header(void* prog_buffer, struct Elf64_Ehdr** header)
{
    int res = 0;

    struct Elf64_Ehdr* elf64_header = (struct Elf64_Ehdr*)kzalloc(sizeof(struct Elf64_Ehdr));
    if (!elf64_header)
    {
        res = -ENOMEM;
        goto out;
    }

    memcpy(elf64_header, prog_buffer, sizeof(struct Elf64_Ehdr));

    res = elf64_valid(elf64_header);
    if (res)
    {
        res = -EUELF;
        goto out;
    }

    elf64_debug_header(elf64_header);
    *header = elf64_header;

out:
    if (res) kfree(elf64_header);
    return res;
}

struct Elf64_Phdr* elf64_read_prog_header(void* data_buffer, struct Elf64_Ehdr* elf_header)
{
    struct Elf64_Phdr* elf_p_header = (struct Elf64_Phdr*)(((char*)data_buffer) + elf_header->e_phoff);
    elf64_debug_prog_header(elf_p_header);
    return elf_p_header;
}
