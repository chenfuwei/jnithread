#include <jni.h>
#include <string>
#include <pthread.h>
#include "../include/Student.h"
#include "../include/JniBase.h"
#include <android/bitmap.h>
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <android/native_window.h>
#include <android/native_window_jni.h>

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

extern "C" JNIEXPORT void
JNICALL
Java_com_picturejni_MainActivity_copyBitmap(JNIEnv *env, jobject object, jobject srcBitmap, jobject desBitmap) {
    JNI_LOGI("copyBitmap call success");
    AndroidBitmapInfo srcInfo;
    int srcInof = AndroidBitmap_getInfo(env, srcBitmap, &srcInfo);
    if (srcInof != ANDROID_BITMAP_RESULT_SUCCESS) {
        JNI_LOGE("getinfo srcbitmap error!");
        return;
    }
    JNI_LOGI("src info width = %d height = %d format = %d", srcInfo.width, srcInfo.height,
             srcInfo.format);
    AndroidBitmapInfo desInfo;
    int desStatus = AndroidBitmap_getInfo(env, desBitmap, &desInfo);
    if(desStatus != ANDROID_BITMAP_RESULT_SUCCESS) {
        JNI_LOGE("getinfo desBitmap error!");
    }
    JNI_LOGI("des info width = %d height = %d format = %d", desInfo.width, desInfo.height,
             desInfo.format);


    void * src, * des;
    int srcLockPixel = AndroidBitmap_lockPixels(env, srcBitmap, &src);
    int desLockPixel = AndroidBitmap_lockPixels(env, desBitmap, &des);
    JNI_LOGI("srcLockPixel = %d, desLockPixel = %d", srcLockPixel, desLockPixel);

    int32_t *int32Src = (int32_t*)src;
    int32_t *int32Des = (int32_t*)des;
    memcpy(int32Des, int32Src, srcInfo.width*srcInfo.height * 4);

    int srcUnLockPixel = AndroidBitmap_unlockPixels(env, srcBitmap);
    int desUnLockPixel = AndroidBitmap_unlockPixels(env, desBitmap);
    JNI_LOGI(" srcUnLockPixel = %d desUnLockPixel = %d", srcUnLockPixel, desUnLockPixel);
}

extern "C" JNIEXPORT void
JNICALL
Java_com_picturejni_MainActivity_jniSurface(JNIEnv *env, jobject object, jobject surface, jobject srcBitmap)
{
    JNI_LOGI("jniSurface");
    AndroidBitmapInfo srcInfo;
    int srcInfoStatus = AndroidBitmap_getInfo(env, srcBitmap, &srcInfo);
    if(srcInfoStatus != ANDROID_BITMAP_RESULT_SUCCESS)
    {
        JNI_LOGI("getInfo src error!");
    }

    JNI_LOGI("srcInfo info width = %d height = %d",srcInfo.width, srcInfo.height);

    int32_t * srcBuf;
    int srcLocStatus = AndroidBitmap_lockPixels(env, srcBitmap, (void**)(&srcBuf));

    ANativeWindow* nativeWindow = ANativeWindow_fromSurface(env, surface);
    ANativeWindow_acquire(nativeWindow);
    ANativeWindow_setBuffersGeometry(nativeWindow, srcInfo.width, srcInfo.height, WINDOW_FORMAT_RGBA_8888);
    ANativeWindow_Buffer window_buffer;
    int windowLock = ANativeWindow_lock(nativeWindow, &window_buffer,NULL);
    if(windowLock == 0) {
        JNI_LOGI("nativewindow info width = %d height = %d format = %d stride = %d",
                 window_buffer.width, window_buffer.height, window_buffer.format,
                 window_buffer.stride);
        int32_t *desbuffer = (int32_t *) window_buffer.bits;
        if (window_buffer.stride == srcInfo.width) {
         memcpy(desbuffer, srcBuf, srcInfo.width * srcInfo.height);
        }else{
            int offet = window_buffer.stride - srcInfo.width;
            for(int i = 0; i < srcInfo.height; i++)
            {
                //memcpy拷贝的是字节数长度
                memcpy(desbuffer + i * window_buffer.stride, srcBuf + i * srcInfo.width, srcInfo.width * 4);
            }
        }
        ANativeWindow_unlockAndPost(nativeWindow);
    }


    ANativeWindow_release(nativeWindow);
    AndroidBitmap_unlockPixels(env, srcBitmap);
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
