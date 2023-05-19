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

    struct Elf64_Ehdr* elf64_header = (struct Elf64_Ehdr*)prog_buffer;
    res = elf64_valid(elf64_header);
    if (res)
    {
        res = -EUELF;
        goto out;
    }

    elf64_debug_header(elf64_header);
    *header = elf64_header;

out:
    return res;
}

static int __parse_elf64_pheader(struct Elf64_Phdr* elf_p_header, struct elf_content* elf_content)
{
    int res = 0;
    if (elf_content->virtual_base_address > (void*)elf_p_header->p_vaddr || elf_content->virtual_base_address == 0x00)
    {
        elf_content->virtual_base_address = (void*)elf_p_header->p_vaddr;
        if (!validate_aligment(elf_content->virtual_base_address))
        {
            res = -EINVARG;
            goto out;
        }
        if (!elf_content->entry) elf_content->entry = (void*)(((char*)elf_content->base_address) + elf_p_header->p_offset);
    }

    uint64_t end_virtual_address = (uint64_t)elf_p_header->p_vaddr + elf_p_header->p_filesz;
    if (elf_content->virtual_end_address < (void*)(end_virtual_address) || elf_content->virtual_end_address == 0x00)
    {
        elf_content->virtual_end_address = (void*)end_virtual_address;
    }
out:
    return res;
}

static struct Elf64_Phdr* __elf_program_header(struct Elf64_Ehdr* elf_header, int i) {
    return (struct Elf64_Phdr*)&(((char*)elf_header) + elf_header->e_phoff)[i];
}

// static int __init_program_content(struct Elf64_Ehdr* elf_header, struct elf_content* elf_content)
// {
//     int res = 0;
//     elf_content->content = kzalloc(elf_content->size);
//     if (!elf_content->content)
//     {
//         res = -ENOMEM;
//         goto out;
//     }
//     int result = -EINVARG;
//     void* physical_base_address = 0;
//     void* physical_end_address = 0;
//     void* base = elf_content->content;
//     for (int i = 0; i < elf_header->e_phnum; i++)
//     {
//         struct Elf64_Phdr* elf_p_header = __elf_program_header(elf_header, i);
//         switch (elf_p_header->p_type)
//         {
//         case PT_LOAD:
//             physical_base_address = (void*)(((uint64_t)elf_content->base_address) + elf_p_header->p_offset);
//             if (!validate_aligment(physical_base_address))
//             {
//                 res = -EINVARG;
//                 goto out;
//             }
//             if (physical_end_address == 0x00) physical_end_address = physical_base_address;
//             base += (physical_base_address - physical_end_address);
//             memcpy(base, physical_base_address, elf_p_header->p_filesz);
//             physical_end_address = (void*)align_up_4k((uint64_t)(physical_base_address)+elf_p_header->p_filesz);
//             result = SUCCESS;
//         }
//     }
// out:
//     if (result)
//     {
//         kfree(elf_content->content);
//         res = result;
//     }
//     return res;
// }

int elf64_parse_pheader(struct Elf64_Ehdr* elf_header, struct elf_content** out_elf)
{
    int res = 0;
    struct elf_content* elf_content = (struct elf_content*)kzalloc(sizeof(struct elf_content));
    if (!elf_content)
    {
        res = -ENOMEM;
        goto out;
    }
    elf_content->base_address = (void*)elf_header;
    for (int i = 0; i < elf_header->e_phnum; i++)
    {
        struct Elf64_Phdr* elf_p_header = __elf_program_header(elf_header, i);
        elf64_debug_prog_header(elf_p_header);
        switch (elf_p_header->p_type)
        {
        case PT_LOAD:
            res = __parse_elf64_pheader(elf_p_header, elf_content);
        }
        if (res) break;
    }
    if (!res) {
        elf_content->size = (size_t)(elf_content->virtual_end_address - elf_content->virtual_base_address);
        // res = __init_program_content(elf_header, elf_content);
    }
    if (!res)    *out_elf = elf_content;
out:
    return res;
}
