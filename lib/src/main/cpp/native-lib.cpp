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