#include <jni.h>
#include <string>
#include <pthread.h>
#include "../include/Student.h"
#include "../include/JniBase.h"

static JavaVM *g_jvm;
Student *student;
extern "C" JNIEXPORT jstring
JNICALL
Java_com_picturejni_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    std::string hello1 = "Hello from C++1";

    student = new Student("chenfuwei", g_jvm);
    std::string name = student->getValue();
    JNI_LOGI("name = %s", name.c_str());
    student->createThead();
    return env->NewStringUTF(name.c_str());
}

JNIEXPORT jint JNI_OnLoad(JavaVM *vm, void *reserved)
{
    JNI_LOGI("JNI_OnLoad vm =%d ", &vm);
    JNIEnv* env = NULL;
    if (vm->GetEnv((void**) &env, JNI_VERSION_1_4) != JNI_OK) {
        return -1;
    }
    g_jvm = vm;
    return JNI_VERSION_1_4;
}
