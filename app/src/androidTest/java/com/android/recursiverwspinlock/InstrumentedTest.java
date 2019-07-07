package com.android.recursiverwspinlock;

import android.content.Context;

import androidx.test.InstrumentationRegistry;
import androidx.test.runner.AndroidJUnit4;

import org.junit.Test;
import org.junit.runner.RunWith;

import static org.junit.Assert.*;

/**
 * Instrumented test, which will execute on an Android device.
 *
 * @see <a href="http://d.android.com/tools/testing">Testing documentation</a>
 */
@RunWith(AndroidJUnit4.class)
public class InstrumentedTest {
    @Test
    public void runTest() {
        assertEquals(runNativeTest(Runtime.getRuntime().availableProcessors(), 1000000000, 100, true), true);
    }

    private native boolean runNativeTest(long threads, long iterations, long depth, boolean randomizeDepth);

    static {
        System.loadLibrary("RecursiveRWSpinlock_LIB");
    }
}
