#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dlfcn.h>
#include <sys/types.h>

#include "checker.h"

int check_all()
{
    int result = 0;
    // Todo: make checker handler array
    return result;
}

int check_cve_2017_0589()
{
	uint8_t m1[3] = {0xf, 0xe3, 0x7f};
	uint8_t m2[2] = {0x47, 0xe3};
    const char *filename = "/system/lib/libstagefright_soft_hevcdec.so";

    void *libso = dlopen(filename, RTLD_LAZY);
	if (!libso)
		return -1;

    const char * symbol = "ihevcd_cabac_init";
    uint8_t *fun_begin = (uint8_t *)dlsym(libso, symbol);
	if (!fun_begin)
		return -1;

    uint8_t *fun_end = fun_begin + 160;
	while (fun_begin <= fun_end) {
		if (*fun_begin == 0xff &&
		    !memcmp(fun_begin + 2, &m1, 3) &&
		    !memcmp(fun_begin + 6, &m2, 2))
			return 0;
		fun_begin += 2;
	}

	return 1;
}
