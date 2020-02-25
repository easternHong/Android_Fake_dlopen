#ifndef DLFCN_COMPAT_H
#define DLFCN_COMPAT_H

#ifndef PROP_VALUE_MAX
#define PROP_VALUE_MAX 92
#endif


extern "C" {
void *dlopen_compat(const char *filename, int flags);

void *dlsym_compat(void *handle, const char *symbol);

int dlclose_compat(void *handle);

const char *dlerror_compat();

int get_sdk_level();
}

#endif //DLFCN_COMPAT_H