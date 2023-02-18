# RecursiveRWSpinlock
A read/write spinlock that suppots read->read, write->read and write->write recursion. Usable on ARM under Android and on x64 under Windows and Linux.
No priority inversion mitigation, no writers prioritization, no FIFO queuing, just plain spinning while the lock is contended.
Tested on a machine with Ryzen 5 2500U under Windows 10 and Ubuntu 18.04 and on a phone with MT6739 under Android 8.1.0.
To run the tests on Android, open the project in Android Studio and run the InstrumentedTest test target.
To run the tests on Windows/Linux, build the project from CMakeLists.txt and use the resulting executable (RecursiveRWSpinlock).
