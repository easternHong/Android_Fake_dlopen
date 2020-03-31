#include <jni.h>
#include <string.h>
#include <dlfcn.h>
#include <android/log.h>
#include <xhook.h>
#include <sstream>
#include <xh_core.h>

#define  LOG_TAG "DUMP_TO_LOG"

#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__);
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__);

extern "C" {

void *dlopen_compat(const char *filename, int flags);

void *dlsym_compat(void *handle, const char *symbol);

int dlclose_compat(void *handle);

const char *dlerror_compat();

int get_sdk_level();

}

extern "C"
JNIEXPORT void JNICALL
Java_com_eastern_hook_What_testDlopen(JNIEnv *env, jobject thiz) {
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
Java_com_eastern_hook_What_invokeFunPoint(JNIEnv *env, jobject thiz) {
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
Java_com_eastern_hook_What_dumpToLog(JNIEnv *env, jobject thiz) {
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

void (*original_Runtime_DumpForSigQuit)(std::ostream &os);

void (*original_BaseMutex_DumpAll)(std::ostream &os);

void (*original_testLog)();


void ProxyDumpForSigQuit(std::ostream &os) {
    LOGE("yes run proxy ProxyDumpForSigQuit")
    original_Runtime_DumpForSigQuit(os);
}

void ProxyBaseMutexDumpAll(std::ostream &os) {
    LOGE("yes run proxy ProxyBaseMutexDumpAll")
    original_BaseMutex_DumpAll(os);
}

void ProxyTestLog() {
    LOGE("yes run proxy ProxyTestLog")
    original_testLog();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_eastern_hook_What_doHook(JNIEnv *env, jobject thiz) {
    xh_core_enable_debug(1);
    const char *so_name = "libart.so";
    LOGI("try to hook :%s", so_name);
    void *soinfo = xhook_elf_open(so_name);
    //DumpAll
    int ret = xhook_hook_symbol(soinfo,
                                "_ZN3art9BaseMutex7DumpAllERNSt3__113basic_ostreamIcNS1_11char_traitsIcEEEE",
                                (void *) ProxyBaseMutexDumpAll,
                                (void **) &original_BaseMutex_DumpAll);
    LOGI("xhook_elf_open hook DumpAll:%d", ret);

    so_name = "libnative-lib.so";
    LOGI("try to hook :%s", so_name);
    soinfo = xhook_elf_open(so_name);
    if (!soinfo) {
        LOGE("hook failed")
        return;
    }
    ret = xhook_hook_symbol(soinfo, "_Z13testDumpToLogv", (void *) ProxyTestLog,
                            (void **) &original_testLog);
    LOGI("xhook_elf_open _Z13testDumpToLogv:%d", ret);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_eastern_hook_What_testLog(JNIEnv *env, jobject thiz) {
    LOGE("Java_com_eastern_hook_What_testLog :");
    testDumpToLog();
}

extern "C"
JNIEXPORT void JNICALL
Java_com_eastern_hook_What_hookDumpAll(JNIEnv *env, jobject thiz) {
    LOGE("Java_com_eastern_hook_What_hookDumpAll :");
    const char *lib_name = "libart.so";
    std::string dump_to_log_sym(
            "_ZN3art9BaseMutex7DumpAllERNSt3__113basic_ostreamIcNS1_11char_traitsIcEEEE");
    void *handle = dlopen_compat(lib_name, RTLD_NOW);
    void (*_dump_all)(void *) = (void (*)(void *)) dlsym_compat(handle, dump_to_log_sym.c_str());
    if (_dump_all) {
        std::ostringstream buffer;
        buffer << "what the hell";
        _dump_all(&buffer);
        LOGE("Java_com_eastern_hook_What_hookDumpAll _dump_all :");
    }
    if (handle) {
        dlclose_compat(handle);
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_eastern_hook_What_doUnHook(JNIEnv *env, jobject thiz) {
    std::string dump_to_log_sym("_ZNK7android13AssetManager29DumpToLogEv");
    const char *so_name = "libart.so";
    void *soinfo = xhook_elf_open(so_name);
    if (!soinfo) {
        LOGE("xhook_elf_open failed:");
        return;
    }
    xhook_hook_symbol(soinfo, "DumpForSigQuit", (void *) original_Runtime_DumpForSigQuit, NULL);
    xhook_hook_symbol(soinfo, "DumpAll", (void *) original_BaseMutex_DumpAll, NULL);
    xhook_elf_close(soinfo);
}