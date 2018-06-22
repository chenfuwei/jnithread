//
// Created by Administrator on 2018/6/22.
//
#include "../include/Student.h"
#include "../include/JniBase.h"
#include <unistd.h>

std::string Student::getName()
{
    return name;
}

Student::Student(std::string name, JavaVM* vm) {
    this->name = name;
    this->vm = vm;
    pthread_mutex_init(&pthreadMutex, NULL);
}

Student::~Student() {
    pthread_mutex_destroy(&pthreadMutex);
}

std::string Student::getValue() {
    return "llllll";
}

void Student::createThead() {
    pthread_create(&pthread, NULL, initThread, (void*)this);

    pthread_create(&pthread1, NULL, initThread, (void*)this);
}

void* Student::initThread(void *args) {
    Student* student = (Student*)args;
    while(true) {
        pthread_once(&student->once, initkey);
        pthread_mutex_lock(&student->pthreadMutex);
        pid_t pid;
        pthread_t tid;
        pid = getpid();
        tid = pthread_self();
        JNIEnv *env = (JNIEnv *) pthread_getspecific(g_thread_key);
        if (!env) {
            JNI_LOGI("pthread_once00 tid = %d", tid);
            if (student->vm->AttachCurrentThread(&env, NULL) == JNI_OK) {
                JNI_LOGI("pthread_once tid11 = %d", tid);
                pthread_setspecific(g_thread_key, env);
            }
        }
        pthread_mutex_unlock(&student->pthreadMutex);
    }

    JNI_LOGI("thead student name = %s", student->getName().c_str());
//    JNI_LOGI("pid = %d, tid = %d", 1, 1);

//    JNI_LOGI("pid = %d, tid = %d", 1, 12);

//    JNI_LOGI("pid = %d, tid = %d", (unsigned int)pid, (unsigned int)tid);
//    pthread_mutex_unlock(&student->pthreadMutex);
    return NULL;
}

void Student::initkey() {
    JNI_LOGI("initkey");
    pthread_key_create(&g_thread_key, destroyKey);
}

void Student::destroyKey(void * arg)
{
    JNI_LOGI("destroyKey");
    JNIEnv* env = (JNIEnv*)arg;
    if(env)
    {
        pthread_setspecific(g_thread_key, NULL);
    }
}

