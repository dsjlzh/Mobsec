//
// Created by gerald on 6/8/17.
//

#include <errno.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/param.h>
#include <elf.h>

#include "hot-patch.h"

int g_patched = 0;

int patch_all()
{
    int result = 0;
    result += patch_linker_is_accessible();
    g_patched = 1;
    return result;
}

int patch_linker_is_accessible()
{
    u_long base;
    u_long sym_off;

    const char *filename = "/system/bin/linker";
    const char *sym_name = "__dl__ZN19android_namespace_t13is_accessibleERKNSt3__112basic_stringIcNS0_11char_traitsIcEENS0_9allocatorIcEEEE";

    if (!(base = dlopen_in_mem(filename)))
        return -1;

    if (!(sym_off = dlsym_in_mem(filename, sym_name)))
        return -1;

    u_long sym_addr = (base + sym_off) & ~1;

    if (mprotect((void *)(sym_addr & 0xFFFFF000), 0x1020, PROT_READ|PROT_WRITE|PROT_EXEC) != 0)
        return -1;

    *(u_int32_t *)sym_addr = 0x46F72001;

    return 0;
}

u_long dlopen_in_mem(const char* filename)
{
    u_long addr;
    char buf[256];
    bool found = false;

    // b19fb000-b1a5a000 r-xp 00000000 103:09 246       /system/bin/linker
    FILE *maps = fopen("/proc/self/maps", "r");
    if (!maps) {
        return NULL;
    }

    while (fgets(buf, sizeof(buf), maps)) {
        if (strstr(buf, "r-xp") && strstr(buf, filename)) {
            found = true;
            break;
        }
    }
    fclose(maps);

    if (!found)
        return NULL;

    if (sscanf(buf, "%lx", &addr) != 1)
        return NULL;

    return addr;
}

u_long dlsym_in_mem(const char *filename, const char *sym_name)
{
    int fd;
    size_t len = 0;

    if ((fd = open(filename, O_RDONLY)) < 0)
        return NULL;

    if ((len = (size_t)lseek(fd, 0, SEEK_END)) <= 0)
        return NULL;

    char *elf = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    if (elf == MAP_FAILED) {
        munmap(elf, len);
        return NULL;
    }

    Elf32_Ehdr *ehdr = (Elf32_Ehdr *)elf;
    Elf32_Shdr *shdr = (Elf32_Shdr *)(elf + ehdr->e_shoff);
    Elf32_Shdr *hdr_shstr = shdr + ehdr->e_shstrndx;
    if (hdr_shstr->sh_type != SHT_STRTAB)
        return NULL;

    char *shstrtab = elf + hdr_shstr->sh_offset;
    char *symtab = NULL;
    char *strtab = NULL;
    int sym_num = 0;
    int i = 0;

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
        return NULL;

    Elf32_Sym *sym = (Elf32_Sym *)symtab + 1;
    u_long offset = 0;
    for (i = 1; i < sym_num; i++) {
        if (strcmp(strtab + sym->st_name, sym_name) == 0) {
            offset = sym->st_value;
            break;
        }
        sym++;
    }

    munmap(elf, len);

    return offset;
}