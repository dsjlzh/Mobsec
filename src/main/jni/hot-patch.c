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
    dl_info_t *info;
    if (!(info = dlopen_in_memory("/system/bin/linker", 0)))
        return -1;

    return 0;
}

void *dlopen_in_memory(const char* filename, int flags)
{
    u_long addr;
    int fd;
    size_t len = 0;
    char *elf;
    Elf32_Ehdr *ehdr;
    Elf32_Shdr *shdr;
    int count = 0;
    dl_info_t *info;
    FILE *maps;
    char buf[256];
    size_t buf_size = 256;
    bool found = false;

    // b19fb000-b1a5a000 r-xp 00000000 103:09 246       /system/bin/linker
    maps = fopen("/proc/self/maps", "r");
    if (!maps) {
        return NULL;
    }

    while (fgets(buf, (int)buf_size, maps)) {
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

    if ((fd = open(filename, O_RDONLY)) < 0)
        return NULL;

    if ((len = (size_t)lseek(fd, 0, SEEK_END)) <= 0)
        return NULL;

    elf = mmap(NULL, len, PROT_READ, MAP_PRIVATE, fd, 0);
    close(fd);
    if (elf == MAP_FAILED) {
        munmap(elf, len);
        return NULL;
    }

    info = (dl_info_t *)malloc(sizeof(dl_info_t));
    memset(info, 0, sizeof(dl_info_t));
    info->base = addr;

    ehdr = (Elf32_Ehdr *)elf;
    shdr = (Elf32_Shdr *)(elf + ehdr->e_shoff);
    while (count < ehdr->e_shnum) {
        switch (shdr->sh_type) {
            case SHT_DYNSYM:
                if (!info->dynsym) {
                    info->dynsym = malloc(shdr->sh_size);
                    memcpy(info->dynsym, elf + shdr->sh_offset, shdr->sh_size);
                    info->dynsym_size = shdr->sh_size >> 4;
                }
                break;
            case SHT_STRTAB:
                if (!info->dynstr) {
                    info->dynstr = malloc(shdr->sh_size);
                    memcpy(info->dynstr, elf + shdr->sh_offset, shdr->sh_size);
                }
                break;
            case SHT_PROGBITS:
                if (info->dynsym && info->dynstr) {
                    info->text = shdr->sh_addr - shdr->sh_offset;
                    count = ehdr->e_shnum;
                }
                break;
            default:
                break;
        }
        count++;
        shdr++;
    }
    munmap(elf, len);

    if (!info->dynstr || !info->dynsym) {
        return NULL;
    }

    return info;
}