#ifndef _HOT_PATCH_H
#define _HOT_PATCH_H

#include <stdbool.h>

extern int g_patched;

bool patch_all();
bool patch_linker();
bool patch_linker_internal(u_long);
u_long dlopen_in_mem(const char *);
bool dlsym_in_mem(const char *, const char *[], u_long[]);

#endif //_HOT_PATCH_H
