#include <jni.h>
#include <string>
#include <dlfcn.h>
#include <android/log.h>

#define  LOG_TAG "DUMP_TO_LOG"

#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__);\
                    printf(__VA_ARGS__)

extern "C" {

void *dlopen_compat(const char *filename, int flags);

void *dlsym_compat(void *handle, const char *symbol);

int dlclose_compat(void *handle);

const char *dlerror_compat();

int get_sdk_level();

}

extern "C"
JNIEXPORT void JNICALL
Java_com_eastern_fake_1dlopen_MainActivity_testDlopen(JNIEnv *env, jobject thiz) {
    int sdk = get_sdk_level();
    std::string lib_name("libandroidfw.so");
    std::string dump_to_log_sym("_ZNK7android13AssetManager29DumpToLogEv");

    if (sdk >= 26) {
        lib_name = "libandroidfw.so";
        void *handle;
        //DumpToLog
        //_ZNK7android13AssetManager29DumpToLogEv
        handle = dlopen_compat(lib_name.c_str(), RTLD_NOW);
        void *(*_dump_to_log_fun)(void *) = (void *(*)(void *)) dlsym_compat(handle,
                                                                             dump_to_log_sym.c_str());
        LOGI("_dump_to_log_fun :%p", _dump_to_log_fun);
        if (handle) {
            dlclose_compat(handle);
        }
    }
}

void testDumpToLog() {
    LOGI("  testDumpToLog   ");
    LOGI("  testDumpToLog   :%s", __func__);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_eastern_fake_1dlopen_MainActivity_invokeFunPoint(JNIEnv *env, jobject thiz) {
    int sdk = get_sdk_level();
    FILE *maps = fopen("/proc/self/maps", "r");
    const char *lib_name = "libnative-lib.so";
    int found = 0;
    char buff[256];
    char *lib_path = NULL;
    while (!found && fgets(buff, sizeof(buff), maps)) {
        if ((strstr(buff, lib_name))) {
            lib_path = buff;
            break;
        }
    }
    char subbuff[128];
    if (lib_path) {
        //"7c64268000-7c64294000 r-xp 00000000 103:03 3277567                       /data/app/com.eastern.dumptolog-PKKzDYcZIK8xKc9ZWgsvcA==/lib/arm64/libnative-lib.so\n"
        int i = 0;
        while (i < strlen(lib_path)) {
            if (lib_path[i] == '/') {
                break;
            }
            i++;
        }
        memcpy(subbuff, &lib_path[i], strlen(lib_path));
        subbuff[strlen(subbuff) - 1] = '\0';
    }
    lib_name = subbuff;
    std::string dump_to_log_sym("_Z13testDumpToLogv");

    if (sdk >= 26) {
        void *handle;
        handle = dlopen_compat(lib_name, RTLD_NOW);
        void *(*_dump_to_log_fun)() = (void *(*)()) dlsym_compat(handle, dump_to_log_sym.c_str());

        _dump_to_log_fun();
        LOGI("_dump_to_log_fun :%p", _dump_to_log_fun);
        if (handle) {
            dlclose_compat(handle);
        }
    }
}



extern "C"
JNIEXPORT void JNICALL
Java_com_eastern_fake_1dlopen_MainActivity_dumpToLog(JNIEnv *env, jobject thiz) {
    int sdk = get_sdk_level();
    const char *lib_name = "libandroidfw.so";
    std::string dump_to_log_sym("_ZNK7android13AssetManager29DumpToLogEv");

    if (sdk >= 26) {
        void *handle;
        //DumpToLog
        //_ZNK7android13AssetManager29DumpToLogEv
        handle = dlopen_compat(lib_name, RTLD_NOW);
        void *(*_dump_to_log_fun)() = (void *(*)()) dlsym_compat(handle, dump_to_log_sym.c_str());
        _dump_to_log_fun();
        LOGI("_dump_to_log_fun :%p", _dump_to_log_fun);
        if (handle) {
            dlclose_compat(handle);
        }
    }
}