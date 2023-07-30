#include "Engine.h"
#include <unistd.h>

#ifdef __vita__
#include <vitasdk.h>
#include <dirent.h>
#include <vitasdk.h>
#include <vitaGL.h>
int _newlib_heap_size_user = 128 * 1024 * 1024;

char patched_fname[256];
void patch_fname(char *fname) {
	if (!strncmp(fname, "(null)/", 7)) {
		sprintf(patched_fname, "ux0:data/beat2x/%s", &fname[7]);
	} else {
		sprintf(patched_fname, "ux0:data/beat2x/%s", fname);
	}
}

extern "C" int __real_remove(const char *fname);
extern "C" int __wrap_remove(const char *fname) {
	printf("remove %s\n", fname);
	if (!strncmp(fname, "ux0:", 4)) {
		return __real_remove(fname);
	} else {
		patch_fname((char *)fname);
	}
	//printf("patched to %s\n", patched_fname);
	return __real_remove(patched_fname);
}

extern "C" FILE *__real_fopen(char *fname, char *mode);
extern "C" FILE *__wrap_fopen(char *fname, char *mode) {
	printf("fopen %s\n", fname);
	if (strlen(fname) == 0)
		return NULL;
	if (!strncmp(fname, "ux0:", 4)) {
		return __real_fopen(fname, mode);
	} else {
		patch_fname(fname);
	}
	//printf("patched to %s\n", patched_fname);
	return __real_fopen(patched_fname, mode);
}

extern "C" int __real_stat(const char *fname, struct stat *buf);
extern "C" int __wrap_stat(const char *fname, struct stat *buf) {
	printf("stat %s\n", fname);
	if (!strncmp(fname, "ux0:", 4)) {
		return __real_stat(fname, buf);
	} else {
		patch_fname((char *)fname);
	}
	//printf("patched to %s\n", patched_fname);
	return __real_stat(patched_fname, buf);
}

extern "C" int __real_scandir(const char *fname, struct dirent ***namelist, int (*select)(const struct dirent *), int (*compar)(const struct dirent **, const struct dirent **));
extern "C" int __wrap_scandir(const char *fname, struct dirent ***namelist, int (*select)(const struct dirent *), int (*compar)(const struct dirent **, const struct dirent **)) {
	if (!strncmp(fname, "ux0:", 4)) {
		return __real_scandir(fname, namelist, select, compar);
	} else {
		patch_fname((char *)fname);
	}
	//printf("patched to %s\n", patched_fname);
	return __real_scandir(patched_fname, namelist, select, compar);
}

extern "C" DIR *__real_opendir(const char *fname);
extern "C" DIR *__wrap_opendir(const char *fname) {
	printf("opendir %s\n", fname);
	if (!strncmp(fname, "ux0:", 4)) {
		return __real_opendir(fname);
	} else {
		patch_fname((char *)fname);
	}
	//printf("patched to %s\n", patched_fname);
	return __real_opendir(patched_fname);
}
#endif

int main ( int argc, char** argv )
{
    Engine engine;

    engine.Init();
    engine.Start();

#ifdef TARGET_GP2X
    chdir("/usr/gp2x");
    execl("/usr/gp2x/gp2xmenu", "/usr/gp2x/gp2xmenu", NULL);
#endif

    return 0;
}
