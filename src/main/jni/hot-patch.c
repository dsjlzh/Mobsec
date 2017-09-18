//
// Created by gerald on 6/8/17.
//

#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <elf.h>

#include "hot-patch.h"

#define SYM_COUNT 2

int g_patched = 0;

bool patch_all()
{
    g_patched = 1;

    if (!patch_linker())
        return false;

    return true;
}

bool patch_linker()
{
    u_long base = 0;
    const char *filename = "/system/bin/linker";

    if (!(base = dlopen_in_mem(filename)))
        return false;

    const char *sym_name[SYM_COUNT] = {
            "__dl__ZN19android_namespace_t13is_accessibleERKNSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE",
            "__dl__ZL13is_greylistedPKcPK6soinfo"};
    u_long sym_off[SYM_COUNT];

    if (dlsym_in_mem(filename, sym_name, sym_off)) {
        for (int i = 0; i < SYM_COUNT; i++)
            if (!patch_linker_internal(base + sym_off[i]))
                return false;
    }

    return true;
}

bool patch_linker_internal(u_long sym_addr)
{
    if (mprotect((void *)(sym_addr & 0xFFFFF000), 0x1000, PROT_READ|PROT_WRITE|PROT_EXEC) != 0)
        return false;

    *(u_int32_t *)sym_addr = 0x46F72001;

    return true;
}

u_long dlopen_in_mem(const char* filename)
{
    u_long addr;
    char buf[256];
    bool found = false;

    // b19fb000-b1a5a000 r-xp 00000000 103:09 246       /system/bin/linker
    FILE *maps = fopen("/proc/self/maps", "r");
    if (!maps)
        return 0;

    while (fgets(buf, sizeof(buf), maps)) {
        if (strstr(buf, "r-xp") && strstr(buf, filename)) {
            found = true;
            break;
        }
    }
    fclose(maps);

    if (!found)
        return 0;

    if (sscanf(buf, "%lx", &addr) != 1)
        return 0;

    return addr;
}

bool dlsym_in_mem(const char *filename, const char *sym_name[],
                    u_long sym_off[])
{
    int fd;
    size_t len = 0;

    if ((fd = open(filename, O_RDONLY)) < 0)
        return false;

    if ((len = (size_t)lseek(fd, 0, SEEK_END)) <= 0)
        return false;

    char *elf = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    if (elf == MAP_FAILED) {
        munmap(elf, len);
        return false;
    }

    Elf32_Ehdr *ehdr = (Elf32_Ehdr *)elf;
    Elf32_Shdr *shdr = (Elf32_Shdr *)(elf + ehdr->e_shoff);
    Elf32_Shdr *hdr_shstr = shdr + ehdr->e_shstrndx;
    if (hdr_shstr->sh_type != SHT_STRTAB)
        return false;

    char *shstrtab = elf + hdr_shstr->sh_offset;
    char *symtab = NULL;
    char *strtab = NULL;
    int sym_num = 0;
    int i = 0, j = 0;

    for (i = 0; i < ehdr->e_shnum; i++) {
        if (strcmp(shstrtab + shdr->sh_name, ".symtab") == 0) {
            symtab = elf + shdr->sh_offset;
            sym_num = shdr->sh_size / shdr->sh_entsize;
        } else if (strcmp(shstrtab + shdr->sh_name, ".strtab") == 0) {
            strtab = elf + shdr->sh_offset;
        }
        shdr++;
    }

    if (!symtab || !strtab)
        return false;

    Elf32_Sym *sym;
    bool found;
    for (j = 0; j < SYM_COUNT; j++) {
        sym = (Elf32_Sym *)symtab + 1;
        found = false;
        for (i = 1; i < sym_num; i++) {
            if (strcmp(strtab + sym->st_name, sym_name[j]) == 0) {
                found = true;
                sym_off[j] = sym->st_value & ~1;
                break;
            }
            sym++;
        }
        if (!found) {
            munmap(elf, len);
            return false;
        }
    }

    munmap(elf, len);
    return true;
}