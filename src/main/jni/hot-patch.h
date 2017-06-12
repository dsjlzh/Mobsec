#ifndef _HOT_PATCH_H
#define _HOT_PATCH_H

extern int g_patched;

struct dl_info {
    u_long base;
    uint8_t *dynstr;
    uint8_t *dynsym;
    uint dynsym_size;
    u_long text;
};

typedef struct dl_info dl_info_t;

int patch_all();
int patch_linker_is_accessible();
void *dlopen_in_memory(const char* , int);

#endif //_HOT_PATCH_H
