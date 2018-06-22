//
// Created by Administrator on 2018/6/22.
//

#ifndef PICTUREJNI_STUDENT_H
#define PICTUREJNI_STUDENT_H

#include <string>
#include <pthread.h>
#include <jni.h>
static pthread_key_t g_thread_key;
class Student{
    JavaVM *vm;
    std::string name;
    pthread_t pthread;
    pthread_t pthread1;
    pthread_mutex_t pthreadMutex;
    pthread_once_t  once = PTHREAD_ONCE_INIT;


private:
    static void* initThread(void* args);
public:
    std::string getName();
    Student(std::string name, JavaVM * vm);
    std::string getValue();
    void createThead();
    ~Student();
    static void initkey();
    static void destroyKey(void * arg);
};
#endif //PICTUREJNI_STUDENT_H
