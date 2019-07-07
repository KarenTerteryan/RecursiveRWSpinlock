#ifdef __ANDROID__

#include "TestJni.h"
#include "Test.h"

jboolean Java_com_android_recursiverwspinlock_InstrumentedTest_runNativeTest(JNIEnv *env, jobject thisPtr,
        jlong threads, jlong iterations, jlong depth, jboolean randDepth) {
    return RunTest(threads, iterations, depth, randDepth, nullptr);
}

#endif
