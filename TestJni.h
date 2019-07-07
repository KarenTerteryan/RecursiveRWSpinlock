#pragma once
#ifdef __ANDROID__
#include <jni.h>
extern "C" JNIEXPORT jboolean JNICALL
Java_com_android_recursiverwspinlock_InstrumentedTest_runNativeTest(JNIEnv*, jobject, jlong threads, jlong iterations, jlong depth, jboolean randDepth);
#endif
