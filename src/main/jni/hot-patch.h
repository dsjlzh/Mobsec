#ifndef _HOT_PATCH_H
#define _HOT_PATCH_H

extern int g_patched;

int patch_all();
int patch_linker_is_accessible();
u_long dlopen_in_mem(const char *);
u_long dlsym_in_mem(const char *, const char *);

#endif //_HOT_PATCH_H
